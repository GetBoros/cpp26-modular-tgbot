//------------------------------------------------------------------------------------------------------------
module;
#include <meta>
#include <print>
#include <optional>
#include <string_view>
#include <type_traits>
module ATest;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
enum class Mouse_Button
{
    LMB,
    RMB
};
//------------------------------------------------------------------------------------------------------------




// Enum_To_String
template <typename enum_type> requires std::is_enum_v<enum_type>
constexpr std::string_view Enum_To_String(enum_type button_val)
{
    std::string_view fallback_str { "Unknown" };

    template for(constexpr auto enumerator_info : std::define_static_array(std::meta::enumerators_of(^^enum_type) ) )
    {
        if(button_val == [:enumerator_info:])  // if Mouse_Button::LMB == Mouse_Button::LMB?
            return std::meta::identifier_of(enumerator_info);
    }

    return fallback_str;  // Return fallback if not matched
}
//------------------------------------------------------------------------------------------------------------




// String_To_Enum
template <typename enum_type> requires std::is_enum_v<enum_type>  // only if enum
constexpr std::optional<enum_type> String_To_Enum(std::string_view name_str)
{
    std::optional<enum_type> result_val { std::nullopt };

    template for(constexpr auto enumerator_info : std::define_static_array(std::meta::enumerators_of(^^enum_type) ) )
    {
        if(name_str == std::meta::identifier_of(enumerator_info) )  // Match input string against reflected static array
            return [:enumerator_info:];
    }

    return result_val;  // Return empty optional on failure
}
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
void Test_Func()
{
    Mouse_Button active_button { Mouse_Button::RMB };
    std::optional<Mouse_Button> parsed_button { std::nullopt };
    std::string_view input_str {"RMB" };

    std::println("Action performed by button: {}", Enum_To_String(active_button) );  // example current active button

    parsed_button = String_To_Enum<Mouse_Button>(input_str);  // try to find enum if find print line
    if(parsed_button.has_value() == true)
        std::println("Parsed successfully: {}", Enum_To_String(parsed_button.value() ) );

    // Enumerate all options at compile time
    std::println("Supported buttons list: ");
    template for(constexpr auto enumerator_info : std::define_static_array(std::meta::enumerators_of(^^Mouse_Button) ) )
    {
        std::println(" - {}", std::meta::identifier_of(enumerator_info));
    }
}
//------------------------------------------------------------------------------------------------------------
