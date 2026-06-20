//------------------------------------------------------------------------------------------------------------
module;

module Lib_Module;
//------------------------------------------------------------------------------------------------------------
import std;
import Lib_Module_Reflection;
import Lib_Module_Concepts;
//------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------
struct SBase
{
    
};
//------------------------------------------------------------------------------------------------------------
struct SDerived : SBase
{
    void Example_Func()
    {
         std::println("Call from SDerived");
    };
};
//------------------------------------------------------------------------------------------------------------
void Print_Only_Integers(std::integral auto value)
{
    std::println("Yes it is integral {}", value);
}
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
void Handle_Module()
{
    SBase base;
    SDerived derived;

    int number = 3, number_second = 6;
    int *ptr = &number;

    Test();
    std::vector<double> vector_val = {1.1, 2.2, 3.3};
    
    Print_Only_Integers(number);

    if constexpr (std::is_integral_v<decltype(number)>)
        std::println("number is an integer type!");

    NLib_Module_Concepts::Print_Smart(number);
    NLib_Module_Concepts::Print_Multiplied_Value(ptr);
    NLib_Module_Concepts::Print_Multiplied_Value(number);
    NLib_Module_Concepts::Compare_Types(number, ptr);
    NLib_Module_Concepts::Compare_Derived(base, derived);
    NLib_Module_Concepts::Print_Custom_Concept(3.0);
    NLib_Module_Concepts::Print_Analize(vector_val);

    NLib_Module_Concepts::Print_If_Interactive(derived);

}
//------------------------------------------------------------------------------------------------------------
