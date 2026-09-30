//------------------------------------------------------------------------------------------------------------
module;

module Example_Templates;
//------------------------------------------------------------------------------------------------------------
import std;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
void Module_First()
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
}
//------------------------------------------------------------------------------------------------------------
void Module_Second()
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
void Module_Third()
{
    std::string_view test_runtime = "Prefix text suffix";
    constexpr std::string_view prefix = "Prefix ";
    constexpr std::string_view suffix = " suffix";
    constexpr std::string_view test_compile_time = "Prefix text suffix";

    if (test_runtime.starts_with(prefix) == true)
    {
        constexpr std::string_view clear_prefix = test_compile_time.substr(prefix.size() );

        test_runtime.remove_prefix(prefix.size() );

        static_assert(clear_prefix == "text suffix", "qwe");
    }

    if(test_runtime.ends_with(suffix) == true)
    {
        constexpr unsigned int new_len = test_compile_time.size() - suffix.size();
        constexpr std::string_view clear_sufix = test_compile_time.substr(0, new_len);
        
        static_assert(clear_sufix == "Prefix text", "qwe");
    }

    std::println("example: {}", test_runtime);
    std::println("example second: {}", test_compile_time);

}
//------------------------------------------------------------------------------------------------------------
void Handle_Example_Templates()
{
    Module_Third();


}
//------------------------------------------------------------------------------------------------------------
