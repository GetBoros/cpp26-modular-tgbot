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
    constexpr AExample_Constexpr example_constexpr(10);  // Help
    
    AExample_Constexpr example_constexpr_runtime(10);

    static_assert(example_constexpr.Constexpr_Value == 10);  // check
    static_assert(example_constexpr.Example_Constexpr_Info() == 20);  // check second

    if (example_constexpr_runtime.Example_Constexpr_Info() == 20)  // result
            return;
            
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
