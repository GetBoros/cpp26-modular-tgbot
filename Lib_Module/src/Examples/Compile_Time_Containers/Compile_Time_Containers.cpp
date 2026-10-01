//------------------------------------------------------------------------------------------------------------
module;

module Compile_Time_Containers;
//------------------------------------------------------------------------------------------------------------
import std;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
void String_View_Example(std::string_view str)
{
    constexpr std::string_view prefix = "Prefix ";
    constexpr std::string_view suffix = " suffix";
    constexpr std::string_view test_compile_time = "Prefix text suffix";

    if (str.starts_with(prefix) == true)  // if str have curr prefix remove it and check result
    {
        constexpr std::string_view clear_prefix = test_compile_time.substr(prefix.size() );

        str.remove_prefix(prefix.size() );  // remove from first index
        std::println("example string remove prefix: {}", str);

        static_assert(clear_prefix == "text suffix", "result not same");  // check result
    }

    if(str.ends_with(suffix) == true)  // if str have curr suffix
    {
        constexpr unsigned int new_len = test_compile_time.size() - suffix.size();  // get len with out suffix
        constexpr std::string_view clear_sufix = test_compile_time.substr(0, new_len);  // cut from 0 to new len

        str.remove_suffix(suffix.size() );  // remove from index
        
        std::println("example string remove suffix: {}", str);

        static_assert(clear_sufix == "Prefix text", "result not same");  // check result
    }

    std::println("example second: {}", test_compile_time);
}
//------------------------------------------------------------------------------------------------------------
consteval auto Get_Array()
{
    std::array<std::string_view, 3> baked_array {};
    std::vector<std::string_view> vec;

    vec.push_back("Hello 1");
    vec.push_back("Hello 2");
    vec.push_back("Hello 3");

    for (int i = 0; i < 3; i++)
        baked_array[i] = vec[i];

    return baked_array;
}
//------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------
void Module_Third_Examples()
{
    constexpr auto arr = Get_Array();

    String_View_Example("Prefix text suffix");

}
//------------------------------------------------------------------------------------------------------------