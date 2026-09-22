//------------------------------------------------------------------------------------------------------------
module;

module Example_Templates;
//------------------------------------------------------------------------------------------------------------
import std;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
void Handle_Example_Templates()
{
    int user_input;
    int result;
    
    user_input = 0;
    result = TFunc_Example<double>(5, 10.5);
    std::println("result {}", result);

    result = TFunc_Example_NTTP<3>(result);
    std::println("showcase NTTP result is {}", result);

    ACTAD_Example ctad_example { "Hello world" };  // CTAD example
    std::println("text {}", ctad_example);

    // Module 2
    Example_Constinit = 88;  // can be changed

    constexpr int test = Example_Constexpr();  // in compile time
    result = [] static consteval { return Example_Constexpr(); } ();  // initialize by value from compile time
    result = [] static consteval { return Example_Branches(25); } ();
    result = Example_Consteval();  // init value from compile time func

    constexpr int example_allocate_mem = Example_Allocate_Mem();
    std::println("result {}", example_allocate_mem);

}
//------------------------------------------------------------------------------------------------------------
