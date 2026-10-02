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
constexpr char RAW_JSON[] = {
    #embed "hero.json"
    , '\0' // add for safty
};
//------------------------------------------------------------------------------------------------------------
constexpr std::string_view JSON_VIEW(RAW_JSON, sizeof(RAW_JSON) - 1);
//------------------------------------------------------------------------------------------------------------
constexpr std::string_view extract_hero_by_number(std::string_view json, unsigned int target_number)
{
    int current_pos = 0;
    constexpr std::string_view key = "\"hero\": \"";  // find key

    for (unsigned int count = 1; count <= target_number; count++)
    {
        auto start = json.find(key, current_pos);  // find key by curr position
        if (start == std::string_view::npos)
            return "Unknown";

        start += key.size();  // jump on key " 
        auto end = json.find("\"", start);  // find closed "
        if (end == std::string_view::npos)
            return "Unknown";

        if (count == target_number)  // if find with target number return name
            return json.substr(start, end - start);

        current_pos = end + 1;  // if not find switch for next and repeat
    }

    return "Unknown";
}
//------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------
void Module_Third_Examples()
{
    constexpr auto arr = Get_Array();
    constexpr std::string_view first_hero = extract_hero_by_number(JSON_VIEW, 1);
    constexpr std::string_view third_hero = extract_hero_by_number(JSON_VIEW, 3);

    static_assert(first_hero == "Saitama", "Error");
    static_assert(third_hero == "Garro", "Error");

    std::println("1-й герой из файла: {}", first_hero);
    std::println("3-й герой из файла: {}", third_hero);

    std::println("text {}", arr);

    //    Ranges и алгоритмы в Compile-Time:

}
//------------------------------------------------------------------------------------------------------------
