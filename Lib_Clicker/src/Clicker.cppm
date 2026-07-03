module;
#include <unistd.h>

export module Clicker;
//------------------------------------------------------------------------------------------------------------
import std;
//------------------------------------------------------------------------------------------------------------
export struct UniqueFd
{
    ~UniqueFd() 
    { 
        if(File_Description >= 0) 
        {
            ::close(File_Description); 
        }
    }
    
    UniqueFd() = default;
    explicit UniqueFd(int f) : File_Description(f) {}

    UniqueFd(const UniqueFd &other) = delete;
    UniqueFd &operator=(const UniqueFd &other) = delete;
    
    UniqueFd(UniqueFd &&other) noexcept : File_Description(std::exchange(other.File_Description, -1)) {}
    
    UniqueFd &operator=(UniqueFd &&other) noexcept
    {
        if(this != &other)
        {
            if(File_Description >= 0) 
            {
                ::close(File_Description);
            }
            File_Description = std::exchange(other.File_Description, -1);
        }
        return *this;
    }
    
    explicit operator int() const { return File_Description; }

    bool Is_Valid() const { return File_Description >= 0; }
    
    void Reset() 
    { 
        if(File_Description >= 0) 
        {
            ::close(File_Description); 
        }
        File_Description = -1; 
    }

    int File_Description = -1;
};
//------------------------------------------------------------------------------------------------------------
export class AClicker
{
public:
    void Init(bool is_clicker = false);

    bool Is_Mouse_With_Wheel(int fd);
    void Hold_Mouse(int uinput_fd, bool is_press, bool is_quiet = false);
    int Create_Virtual_Mouse();
};
//------------------------------------------------------------------------------------------------------------