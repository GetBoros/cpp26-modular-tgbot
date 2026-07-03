module;
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <linux/uinput.h>

module Clicker;
//------------------------------------------------------------------------------------------------------------
import std;
//------------------------------------------------------------------------------------------------------------

// AClicker
void AClicker::Init(bool is_clicker)
{
    // 1.0. Declaration of variables
    bool is_lmb_held = false;                                                  // 1 byte
    std::atomic<bool> is_clicking_active = false;                              // 1 byte
    int loop_index = 0;                                                        // 4 bytes
    UniqueFd temp_fd;                                                          // 4 bytes (wraps int)
    UniqueFd mouse_fd;                                                         // 4 bytes (wraps int)
    UniqueFd uinput_fd;                                                        // 4 bytes (wraps int)
    std::jthread click_thread;                                                 // 8 bytes (handle wrapper)
    struct input_event ev;                                                     // 24 bytes (event structure)
    std::string path;                                                          // 32 bytes (standard string)
    char name[256] = {0};                                                      // 256 bytes (character array)

    // 1.1. Initialize virtual mouse device
    uinput_fd = UniqueFd{Create_Virtual_Mouse()};

    if(uinput_fd.Is_Valid() != true)
    {
        std::println(stderr, "Error: Cannot create virtual mouse /dev/uinput");

        return;
    }

    // 1.2. Identify and connect physical mouse device
    for(loop_index = 0; loop_index < 32; loop_index++)
    {
        path = std::format("/dev/input/event{}", loop_index);
        temp_fd = UniqueFd{open(path.c_str(), O_RDONLY)};
        
        if(temp_fd.Is_Valid() == true)
        {
            if(Is_Mouse_With_Wheel(static_cast<int>(temp_fd)) == true)
            {
                ioctl(static_cast<int>(temp_fd), EVIOCGNAME(sizeof(name)), name);
                std::println("Connected to physical mouse: {}", name);

                mouse_fd = std::move(temp_fd);                                 // Move descriptor ownership

                break;
            }
        }
    }

    if(mouse_fd.Is_Valid() != true)
    {
        std::println(stderr, "Error: Physical mouse not found!");

        return;
    }

    // 2.0. Display user control guidelines
    std::println("--------------------------------------------------");
    std::println("CONTROLS:");
    if(is_clicker == true)
    {
        std::println("Wheel UP   -> Start Auto-Clicker");
        std::println("Wheel DOWN -> Stop Auto-Clicker & Exit program");
    }
    else
    {
        std::println("Wheel UP   -> Hold LMB");
        std::println("Wheel DOWN -> Release LMB & Exit program");
    }
    std::println("LMB/RMB Physical Click -> Release / Stop Clicker");
    std::println("--------------------------------------------------");

    // 3.0. Main event loop processing
    while(read(static_cast<int>(mouse_fd), &ev, sizeof(ev)) > 0)
    {
        if( (ev.type == EV_REL) && (ev.code == REL_WHEEL) )
        {
            if(ev.value > 0)                                                   // Wheel rolled up
            {
                std::println("1 (Wheel up)");

                if(is_clicker == true)
                {
                    if(is_clicking_active == false)
                    {
                        is_clicking_active = true;
                        click_thread = std::jthread([this, uinput_fd_val = static_cast<int>(uinput_fd), &is_clicking_active]()
                        {
                            while(is_clicking_active == true)
                            {
                                Hold_Mouse(uinput_fd_val, true, true);
                                std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Brief press
                                Hold_Mouse(uinput_fd_val, false, true);
                                std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Delay between clicks
                            }
                        });
                        std::println(">>> AUTO-CLICKER STARTED! (Roll Wheel DOWN or click physical mouse to stop)");
                    }
                }
                else
                {
                    if(is_lmb_held != true)
                    {
                        Hold_Mouse(static_cast<int>(uinput_fd), true, false);
                        is_lmb_held = true;
                    }
                }
            } 
            else if(ev.value < 0)                                              // Wheel rolled down
            {
                std::println("0 (Wheel down)");

                if(is_clicker == true)
                {
                    if(is_clicking_active == true)
                    {
                        is_clicking_active = false;
                        if(click_thread.joinable() == true)
                        {
                            click_thread.join();
                        }
                    }
                }
                else
                {
                    if(is_lmb_held == true)
                    {
                        Hold_Mouse(static_cast<int>(uinput_fd), false, false);
                    }
                }

                break; 
            }
        }

        if( (ev.type == EV_KEY) && ( (ev.code == BTN_LEFT) || (ev.code == BTN_RIGHT) ) )
        {
            if(ev.value == 1)                                                  // Physical click detected
            {
                if(is_clicker == true)
                {
                    if(is_clicking_active == true)
                    {
                        std::println("Physical click detected. Cancelling auto-click.");
                        is_clicking_active = false;
                        if(click_thread.joinable() == true)
                        {
                            click_thread.join();
                        }
                    }
                }
                else
                {
                    if(is_lmb_held == true)
                    {
                        std::println("Physical click detected. Cancelling hold.");
                        Hold_Mouse(static_cast<int>(uinput_fd), false, false);
                        is_lmb_held = false;
                    }
                }
            }
        }
    }

    // 4.0. Resource deallocation and termination
    ioctl(static_cast<int>(uinput_fd), UI_DEV_DESTROY);
    
    std::println("Program finished.");
}
//------------------------------------------------------------------------------------------------------------
bool AClicker::Is_Mouse_With_Wheel(int fd)
{
    // 1.0. Declaration of variables
    unsigned char rel_bitmask[REL_MAX/8 + 1];                                  // 2 bytes
    unsigned char evtype_bitmask[EV_MAX/8 + 1];                                // 8 bytes

    // 1.1. Initialization and payload preparation
    std::memset(evtype_bitmask, 0, sizeof(evtype_bitmask));
    
    // 1.2. Execute queries and check wheel presence
    if(ioctl(fd, EVIOCGBIT(0, sizeof(evtype_bitmask)), evtype_bitmask) < 0)
    {
        return false;
    }

    if( (evtype_bitmask[EV_REL / 8] & (1 << (EV_REL % 8))) != 0 )
    {
        std::memset(rel_bitmask, 0, sizeof(rel_bitmask));
        
        if(ioctl(fd, EVIOCGBIT(EV_REL, sizeof(rel_bitmask)), rel_bitmask) < 0)
        {
            return false;
        }

        if( (rel_bitmask[REL_WHEEL / 8] & (1 << (REL_WHEEL % 8))) != 0 ) 
        {
            return true;
        }
    }

    return false;
}
//------------------------------------------------------------------------------------------------------------
void AClicker::Hold_Mouse(int uinput_fd, bool is_press, bool is_quiet)
{
    // 1.0. Declaration of variables
    struct input_event ie;                                                     // 24 bytes

    // 1.1. Prepare and send key press event
    std::memset(&ie, 0, sizeof(ie));
    ie.type = EV_KEY;
    ie.code = BTN_LEFT;
    ie.value = is_press ? 1 : 0;
    write(uinput_fd, &ie, sizeof(ie));

    // 1.2. Send synchronization report event
    std::memset(&ie, 0, sizeof(ie));
    ie.type = EV_SYN;
    ie.code = SYN_REPORT;
    ie.value = 0;
    write(uinput_fd, &ie, sizeof(ie));

    // 1.3. Print operation status if quiet mode is disabled
    if(is_quiet == false)
    {
        if(is_press == true)
        {
            std::println(">>> LMB HELD! (To cancel, click the physical mouse)");
        }
        else
        {
            std::println(">>> LMB RELEASED!");
        }
    }
}
//------------------------------------------------------------------------------------------------------------
int AClicker::Create_Virtual_Mouse()
{
    // 1.0. Declaration of variables
    int fd;                                                                    // 4 bytes
    struct uinput_user_dev uud;                                                // 84 bytes

    // 1.1. Open user input interface
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    if(fd < 0)
    {
        return -1;
    }

    // 1.2. Configure supported virtual mouse buttons
    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
    
    // 1.3. Prepare virtual device identity structure
    std::memset(&uud, 0, sizeof(uud));
    std::snprintf(uud.name, UINPUT_MAX_NAME_SIZE, "Virtual Clicker");

    uud.id.bustype = BUS_USB;
    uud.id.vendor  = 0x1234;
    uud.id.product = 0x5678;
    uud.id.version = 1;

    // 1.4. Build and register the virtual device
    write(fd, &uud, sizeof(uud));
    ioctl(fd, UI_DEV_CREATE);
    
    sleep(1);                                                                  // Allow the system to register device node
    
    return fd;
}
//------------------------------------------------------------------------------------------------------------