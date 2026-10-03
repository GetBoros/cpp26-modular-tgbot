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
    constexpr int space_count = std::ranges::count(prefix, ' ');
    constexpr bool is_valid_format = std::ranges::all_of(prefix, [](char c)
    {
        return (c >= 'A' && c <= 'Z') || (c == '_');  // return true if all upper case
    } );

    static_assert(space_count == 1);  // if one space
    static_assert(is_valid_format == false);  // if not all symbols are capsed like PRE_FIX
    static_assert(std::ranges::contains(prefix, 'P') );

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
consteval int Return_Sum()
{
    std::vector<int> vec = { 1, 5, 8, 9 };

    return std::ranges::fold_left(vec, 0, std::plus<> {} );
}
//------------------------------------------------------------------------------------------------------------
consteval bool Check_Containt_Num(const int num)
{
    std::vector<int> vec = { 1, 5, 8, 9 };

    return std::ranges::contains(vec, num);
}
//------------------------------------------------------------------------------------------------------------
consteval auto Get_Sorted_Array()
{
    std::array<std::string_view, 4> baked_array {};
    std::vector<std::string_view> str_vector { "banana", "apple", "cherry", "date", "apple" };

    std::ranges::sort(str_vector);
    auto duplicate_tail = std::ranges::unique(str_vector);
    str_vector.erase(duplicate_tail.begin(), duplicate_tail.end() );
    std::ranges::sort(str_vector, std::ranges::greater {} );  // from Z to A
    std::ranges::sort(str_vector, {}, &std::string_view::size);  // sort by size( length )

    std::ranges::copy(str_vector, baked_array.begin() );

    // for (int i = 0; i < 4; i++)
    //     baked_array[i] = str_vector[i];

    return baked_array;
}
//------------------------------------------------------------------------------------------------------------
constexpr char Raw_Json[] = {
    #embed "hero.json"
    , '\0' // add for safty
};
//------------------------------------------------------------------------------------------------------------
constexpr std::string_view Json_View(Raw_Json, sizeof(Raw_Json) - 1);
//------------------------------------------------------------------------------------------------------------
constexpr std::string_view Extract_Hero_By_Number(std::string_view json, unsigned int target_number)
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
void Compile_Time_Containers_Preview()
{
    constexpr bool result = Check_Containt_Num(8);
    constexpr auto arr = Get_Sorted_Array();
    constexpr std::string_view first_hero = Extract_Hero_By_Number(Json_View, 1);
    constexpr std::string_view third_hero = Extract_Hero_By_Number(Json_View, 3);

    static_assert(first_hero == "Saitama", "Error");
    static_assert(third_hero == "Garro", "Error");

    std::println("First hero from file: {}", first_hero);
    std::println("Third hero from file: {}", third_hero);
    std::println("Show example with sorted array {}", arr);
    std::println("result {}", result);
    std::println("integers sum: {}", Return_Sum() );

}
//------------------------------------------------------------------------------------------------------------
