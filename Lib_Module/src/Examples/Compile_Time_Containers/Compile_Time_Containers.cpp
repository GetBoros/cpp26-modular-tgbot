//------------------------------------------------------------------------------------------------------------
module;

module Compile_Time_Containers;
//------------------------------------------------------------------------------------------------------------
import std;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
void Module_Third_Examples()
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