//------------------------------------------------------------------------------------------------------------
module;
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <linux/uinput.h>
module Clicker;
//------------------------------------------------------------------------------------------------------------
import std;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
struct UniqueFd
{
    ~UniqueFd() { if (File_Description >= 0) ::close(File_Description); }
    UniqueFd() = default;
    explicit UniqueFd(int f) : File_Description(f) {}

    // No copy, only move || reference
    UniqueFd(const UniqueFd &) = delete;
    UniqueFd &operator=(const UniqueFd &) = delete;
    UniqueFd(UniqueFd &&other) noexcept : File_Description(std::exchange(other.File_Description, -1) ) { }
    UniqueFd &operator=(UniqueFd &&other) noexcept
    {
        if (this != &other)
        {
            if (File_Description >= 0) ::close(File_Description);
            File_Description = std::exchange(other.File_Description, -1);
        }
        return *this;
    }
    explicit operator int() const { return File_Description; }

    bool Is_Valid() const { return File_Description >= 0; }
    void Reset() { if (File_Description >= 0) ::close(File_Description); File_Description = -1; }

    int File_Description = -1;

};
//------------------------------------------------------------------------------------------------------------




// AClicker
void AClicker::Init()
{
    UniqueFd mouse_fd;
    UniqueFd uinput_fd { Create_Virtual_Mouse() };

    if (uinput_fd.Is_Valid() != true)
    {
        std::println(stderr, "Error: Cannot create virtual mouse /dev/uinput");

        return;
    }

    for (int i = 0; i < 32; i++)  // Try to find a physical mouse
    {
        // Format the path according to C++20 standard without unnecessary memory allocations
        std::string path = std::format("/dev/input/event{}", i);
        UniqueFd temp_fd { open(path.c_str(), O_RDONLY) };
        
        if (temp_fd.Is_Valid() )
        {
            if (Is_Mouse_With_Wheel(static_cast<int>(temp_fd) ) )
            {
                char name[256] = "Unknown";
                
                ioctl(static_cast<int>(temp_fd), EVIOCGNAME(sizeof(name) ), name);
                std::println("Connected to physical mouse: {}", name);

                mouse_fd = std::move(temp_fd); // Move ownership of the file descriptor

                break;
            }
        }
    }

    if (mouse_fd.Is_Valid() != true)
    {
        std::println(stderr, "Error: Physical mouse not found!");

        return;
    }

    std::println("--------------------------------------------------");
    std::println("CONTROLS:");
    std::println("Wheel UP -> Hold LMB");
    std::println("LMB/PMB Click -> Release LMB");
    std::println("Wheel DOWN -> Exit program");
    std::println("--------------------------------------------------");

    bool is_lmb_held = false;
    struct input_event ev;

    while (read(static_cast<int>(mouse_fd), &ev, sizeof(ev) ) > 0)  // main loop to read events from the physical mouse
    {
        if (ev.type == EV_REL && ev.code == REL_WHEEL)
        {
            if (ev.value > 0)  // wheel up
            {
                std::println("1 (Wheel up)");
                if (!is_lmb_held)
                {
                    Hold_Mouse(static_cast<int>(uinput_fd), true);
                    is_lmb_held = true;
                }
            } 
            else if (ev.value < 0)  // wheel down
            {
                std::println("0 (Wheel down)");
                if (is_lmb_held)
                    Hold_Mouse(static_cast<int>(uinput_fd), false);

                break; 
            }
        }

        if (ev.type == EV_KEY && (ev.code == BTN_LEFT || ev.code == BTN_RIGHT) )
        {
            if (ev.value == 1 && is_lmb_held)
            {
                std::println("Physical click detected. Cancelling hold.");

                Hold_Mouse(static_cast<int>(uinput_fd), false);

                is_lmb_held = false;
            }
        }
    }

    // Clean up and destroy the virtual mouse device
    ioctl(static_cast<int>(uinput_fd), UI_DEV_DESTROY);
    
    std::println("Program finished.");
}

//------------------------------------------------------------------------------------------------------------

bool AClicker::Is_Mouse_With_Wheel(int fd)
{
    unsigned char evtype_bitmask[EV_MAX/8 + 1];
    unsigned char rel_bitmask[REL_MAX/8 + 1];

    std::memset(evtype_bitmask, 0, sizeof(evtype_bitmask));
    
    if (ioctl(fd, EVIOCGBIT(0, sizeof(evtype_bitmask)), evtype_bitmask) < 0)
        return false;

    if (evtype_bitmask[EV_REL / 8] & (1 << (EV_REL % 8)))
    {
        std::memset(rel_bitmask, 0, sizeof(rel_bitmask));
        
        if (ioctl(fd, EVIOCGBIT(EV_REL, sizeof(rel_bitmask)), rel_bitmask) < 0)
            return false;

        if (rel_bitmask[REL_WHEEL / 8] & (1 << (REL_WHEEL % 8))) {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------------------------------------

void AClicker::Hold_Mouse(int uinput_fd, bool is_press)
{
    struct input_event ie;
    
    std::memset(&ie, 0, sizeof(ie));
    ie.type = EV_KEY;
    ie.code = BTN_LEFT;
    ie.value = is_press ? 1 : 0;
    write(uinput_fd, &ie, sizeof(ie));

    std::memset(&ie, 0, sizeof(ie));
    ie.type = EV_SYN;
    ie.code = SYN_REPORT;
    ie.value = 0;
    write(uinput_fd, &ie, sizeof(ie));

    if (is_press)
        std::println(">>> ЛКМ ЗАЖАТА! (Для отмены кликни физической мышкой)");
    else
        std::println(">>> ЛКМ ОТПУЩЕНА!");
}

//------------------------------------------------------------------------------------------------------------

int AClicker::Create_Virtual_Mouse()
{
    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    struct uinput_user_dev uud;

    if (fd < 0)
        return -1;

    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
    
    std::memset(&uud, 0, sizeof(uud));
    std::snprintf(uud.name, UINPUT_MAX_NAME_SIZE, "Virtual Clicker");

    uud.id.bustype = BUS_USB;
    uud.id.vendor  = 0x1234;
    uud.id.product = 0x5678;
    uud.id.version = 1;

    write(fd, &uud, sizeof(uud));
    ioctl(fd, UI_DEV_CREATE);
    
    sleep(1);
    
    return fd;
}
//------------------------------------------------------------------------------------------------------------

