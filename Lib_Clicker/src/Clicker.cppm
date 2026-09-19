module;
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include <atomic>
#include <chrono>
#include <cstring>
#include <format>
#include <print>
#include <thread>
#include <utility>

export module Clicker;

export struct Trigger 
{
    uint16_t type;
    uint16_t code;
    int32_t  val;

    bool Matches(const input_event& ev) const 
    {
        if (ev.type != type || ev.code != code) return false;
        return (type == EV_REL) ? (ev.value > 0) : (ev.value == val);
    }
};

export constexpr Trigger Key(uint16_t code) { return { EV_KEY, code, 1 }; }
export constexpr Trigger WheelUp()          { return { EV_REL, REL_WHEEL, 1 }; }

// -----------------------------------------------------------------------------
// КОНФИГ
// -----------------------------------------------------------------------------
export struct ClickerConfig 
{
    // Кнопки активации
    inline static Trigger Start_LMB_Trigger   = Key(KEY_KP7);   // Num 7: Спам ЛКМ
    inline static Trigger Hold_LMB_Trigger    = Key(KEY_KP8);   // Num 8: Зажатие ЛКМ (Hold)
    inline static Trigger Start_Combo_Trigger = Key(KEY_KP9);   // Num 9: Комбо (ЛКМ + ПКМ раз в 1 сек)
    inline static Trigger Exit_Trigger        = Key(KEY_KP2);   // Num 2: Выход
    inline static Trigger Stop_LMB            = Key(BTN_LEFT);  // Физический сброс
    inline static Trigger Stop_RMB            = Key(BTN_RIGHT);

    // Тайминги (мс)
    inline static int Press_Time_MS   = 20;                     // Длительность зажатия клика
    inline static int LMB_Delay_MS    = 250;                     // Пауза между ударами ЛКМ
    inline static int RMB_Interval_MS = 1000;                   // Пауза между ударами ПКМ (1 сек)
};

export struct UniqueFd 
{
    int fd = -1;
    ~UniqueFd() { Reset(); }
    UniqueFd() = default;
    explicit UniqueFd(int f) : fd(f) {}
    UniqueFd(UniqueFd&& o) noexcept : fd(std::exchange(o.fd, -1)) {}
    UniqueFd& operator=(UniqueFd&& o) noexcept { std::swap(fd, o.fd); return *this; }
    operator int() const { return fd; }
    bool Is_Valid() const { return fd >= 0; }
    void Reset() { if (fd >= 0) ::close(std::exchange(fd, -1)); }
};

export class AClicker 
{
public:
    void Init();

private:
    static bool Has_Bit(int fd, int type, int code) 
    {
        unsigned char mask[KEY_MAX / 8 + 1]{};
        return ioctl(fd, EVIOCGBIT(type, sizeof(mask)), mask) >= 0 && (mask[code / 8] & (1 << (code % 8)));
    }

    static void Send_Mouse_Event(int fd, uint16_t button, int value) 
    {
        input_event evs[2] = {
            {.type = EV_KEY, .code = button, .value = value},
            {.type = EV_SYN, .code = SYN_REPORT, .value = 0}
        };
        ::write(fd, evs, sizeof(evs));
    }

    static int Create_Virtual_Mouse() 
    {
        int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
        if (fd < 0) return -1;

        ioctl(fd, UI_SET_EVBIT, EV_KEY);
        ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
        ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);

        uinput_user_dev uud{.id = {.bustype = BUS_USB, .vendor = 0x1234, .product = 0x5678, .version = 1}};
        std::strncpy(uud.name, "Virtual Clicker", UINPUT_MAX_NAME_SIZE);
        
        ::write(fd, &uud, sizeof(uud));
        ioctl(fd, UI_DEV_CREATE);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return fd;
    }
};

void AClicker::Init() 
{
    UniqueFd uinput_fd{Create_Virtual_Mouse()};
    if (!uinput_fd.Is_Valid()) 
    {
        std::println(stderr, "Error: Cannot create /dev/uinput");
        return;
    }

    UniqueFd mouse_fd, keyboard_fd;

    for (int i = 0; i < 64 && (!mouse_fd.Is_Valid() || !keyboard_fd.Is_Valid()); ++i) 
    {
        UniqueFd fd{open(std::format("/dev/input/event{}", i).c_str(), O_RDONLY)};
        if (!fd.Is_Valid()) continue;

        char name[256]{"Unknown"};
        ioctl(fd, EVIOCGNAME(sizeof(name)), name);

        if (!mouse_fd.Is_Valid() && Has_Bit(fd, EV_KEY, BTN_LEFT) && Has_Bit(fd, EV_REL, REL_X)) 
        {
            std::println("Connected to physical mouse: {}", name);
            mouse_fd = std::move(fd);
        } 
        else if (!keyboard_fd.Is_Valid() && Has_Bit(fd, EV_KEY, KEY_ENTER) && Has_Bit(fd, EV_KEY, KEY_SPACE)) 
        {
            std::println("Connected to physical keyboard: {}", name);
            keyboard_fd = std::move(fd);
        }
    }

    if (!mouse_fd.Is_Valid() || !keyboard_fd.Is_Valid()) 
    {
        std::println(stderr, "Error: Physical mouse or keyboard not found!");
        return;
    }

    std::println("--------------------------------------------------");
    std::println("CONTROLS:");
    std::println("Num 7         -> Fast LMB spam");
    std::println("Num 8         -> Hold LMB");
    std::println("Num 9         -> COMBO (LMB + RMB every 1s)");
    std::println("Phys Click    -> Stop / Release");
    std::println("Num 2         -> Exit");
    std::println("--------------------------------------------------");

    bool is_lmb_held = false;
    std::atomic<bool> is_clicking_active = false;
    std::jthread click_thread;

    auto stop_actions = [&]() 
    {
        // 1. Остановка любого спама кликов
        if (is_clicking_active) 
        {
            is_clicking_active = false;
            if (click_thread.joinable()) click_thread.join();
        }
        // 2. Сброс зажатия ЛКМ, если кнопка удерживалась
        if (is_lmb_held) 
        {
            Send_Mouse_Event(uinput_fd, BTN_LEFT, 0);
            is_lmb_held = false;
            std::println(">>> LMB RELEASED!");
        }
    };

    auto start_clicker = [&](bool with_rmb) 
    {
        stop_actions(); // сбрасываем удержание или предыдущий кликер
        is_clicking_active = true;

        click_thread = std::jthread([u_fd = static_cast<int>(uinput_fd), with_rmb, &is_clicking_active]() 
        {
            using clock = std::chrono::steady_clock;
            auto last_rmb_time = clock::now();
            auto rmb_interval = std::chrono::milliseconds(ClickerConfig::RMB_Interval_MS);

            while (is_clicking_active) 
            {
                Send_Mouse_Event(u_fd, BTN_LEFT, 1);
                std::this_thread::sleep_for(std::chrono::milliseconds(ClickerConfig::Press_Time_MS));
                Send_Mouse_Event(u_fd, BTN_LEFT, 0);

                if (with_rmb) 
                {
                    auto now = clock::now();
                    if (now - last_rmb_time >= rmb_interval) 
                    {
                        Send_Mouse_Event(u_fd, BTN_RIGHT, 1);
                        std::this_thread::sleep_for(std::chrono::milliseconds(ClickerConfig::Press_Time_MS));
                        Send_Mouse_Event(u_fd, BTN_RIGHT, 0);
                        last_rmb_time = now;
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(ClickerConfig::LMB_Delay_MS));
            }
        });
    };

    pollfd fds[2] = {
        {.fd = mouse_fd, .events = POLLIN},
        {.fd = keyboard_fd, .events = POLLIN}
    };

    input_event ev;
    bool keep_running = true;

    while (keep_running && poll(fds, 2, -1) > 0) 
    {
        for (int i = 0; i < 2; ++i) 
        {
            if ((fds[i].revents & POLLIN) && read(fds[i].fd, &ev, sizeof(ev)) > 0) 
            {
                // Режим 1: Спам только ЛКМ (Num 7)
                if (ClickerConfig::Start_LMB_Trigger.Matches(ev)) 
                {
                    std::println(">>> MODE: FAST LMB STARTED!");
                    start_clicker(false);
                } 
                // Режим 2: Зажать и держать ЛКМ (Num 8)
                else if (ClickerConfig::Hold_LMB_Trigger.Matches(ev)) 
                {
                    if (!is_lmb_held) 
                    {
                        stop_actions();
                        Send_Mouse_Event(uinput_fd, BTN_LEFT, 1);
                        is_lmb_held = true;
                        std::println(">>> MODE: LMB HELD!");
                    }
                }
                // Режим 3: Комбо ЛКМ + ПКМ (Num 9)
                else if (ClickerConfig::Start_Combo_Trigger.Matches(ev)) 
                {
                    std::println(">>> MODE: COMBO (LMB + RMB/1s) STARTED!");
                    start_clicker(true);
                } 
                // Сброс всего (физический клик мыши)
                else if (ClickerConfig::Stop_LMB.Matches(ev) || ClickerConfig::Stop_RMB.Matches(ev)) 
                {
                    if (is_clicking_active || is_lmb_held) 
                    {
                        std::println("Physical click detected. Stopping.");
                        stop_actions();
                    }
                } 
                // Выход (Num 2)
                else if (ClickerConfig::Exit_Trigger.Matches(ev)) 
                {
                    std::println("Exit pressed -> Closing...");
                    stop_actions();
                    keep_running = false;
                    break;
                }
            }
        }
    }

    ioctl(uinput_fd, UI_DEV_DESTROY);
    std::println("Program finished.");
}
