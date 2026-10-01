//------------------------------------------------------------------------------------------------------------
module;

module Compile_Time;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
void Compile_Time_Preview()
{
    int result;

    Example_Constinit = 88;  // can be changed

    constexpr int test = Example_Constexpr();  // in compile time
    result = [] static consteval { return Example_Constexpr(); } ();  // initialize by value from compile time
    result = [] static consteval { return Example_Branches(25); } ();
    result = Example_Consteval();  // init value from compile time func

    constexpr int example_allocate_mem = Example_Allocate_Mem();
    std::println("result {}", example_allocate_mem);

    AConstexpr_Example constexpr_example_runtime(5);
    constexpr AConstexpr_Example constexpr_example_compile_time(8);
}
//------------------------------------------------------------------------------------------------------------
