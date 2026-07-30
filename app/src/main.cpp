//------------------------------------------------------------------------------------------------------------
#include <main.h>
//------------------------------------------------------------------------------------------------------------
import Lib_Module;
import Lib_TGBot;
import Lib_Clicker;
//------------------------------------------------------------------------------------------------------------





//------------------------------------------------------------------------------------------------------------
void Temp()
{
    constexpr int value = 10;
    constexpr AExample_Constexpr example_constexpr(value);  // Help
    
    AExample_Constexpr example_constexpr_runtime(value);  // temp

    static_assert(example_constexpr.Constexpr_Value == value);  // check
    static_assert(example_constexpr.Example_Constexpr_Info() == 20);  // check second

    if (example_constexpr_runtime.Example_Constexpr_Info() == 20)  // result
            return;  // Done today test almost
            
}
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
int main()
{
    // Handle_Clicker();
    // Handle_TGBot();  // Bot for shop
    Handle_Module();  // Bot for shop
    
    return 0;
}
//------------------------------------------------------------------------------------------------------------
