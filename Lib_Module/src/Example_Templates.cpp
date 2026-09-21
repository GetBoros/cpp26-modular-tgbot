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
    constexpr int test = Example_Constexpr();
    result = [] static consteval { return Example_Constexpr(); } ();
    result = Example_Consteval();

    result = [] static consteval { return Example_Test(25); } ();

}
//------------------------------------------------------------------------------------------------------------
