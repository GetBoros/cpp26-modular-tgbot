//------------------------------------------------------------------------------------------------------------
module;
#include <meta>
#include <concepts>
#include <print>
module Lib_Module_Reflection;
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




// Set_Test
template <typename type_name> void Set_Test(const type_name &object)
{
    constexpr std::meta::info meta_info = ^^type_name;
    constexpr auto access_ctx = std::meta::access_context::unchecked();
    static constexpr auto fields = std::define_static_array(std::meta::nonstatic_data_members_of(meta_info, access_ctx) );
    static constexpr auto members = std::define_static_array(std::meta::members_of(meta_info, access_ctx) );

    // 1.0. Print Obj fields
    template for(constexpr auto field : fields)
    {
        constexpr auto member_name = std::meta::identifier_of(field);  // Caching field name

        std::println("Field: {}, Value: {}", member_name, object.[:field:]);
    }

    // 2.0. Print obj methods
    template for(constexpr auto member : members)
    {
        constexpr bool has_identifier = std::meta::has_identifier(member);
        
        if constexpr(has_identifier == true)
        {
            constexpr auto member_name = std::meta::identifier_of(member);  // Caching member name
            constexpr bool is_func = std::meta::is_function(member);

            if constexpr(is_func == true)
                std::println("Method found: {}", member_name);

        }
    }
}
//------------------------------------------------------------------------------------------------------------




// Search_Word_In_Object
template <typename type_name> 
bool Search_Word_In_Object(const type_name &obj, const char *search_word )
{
    constexpr std::meta::info meta_info = ^^type_name;  // Cache meta-information about the type_name
    constexpr auto access_ctx = std::meta::access_context::unchecked();  // and private get
    static constexpr auto members = std::define_static_array(std::meta::nonstatic_data_members_of(meta_info, access_ctx) );

    template for(constexpr auto member : members)  // Iterate through all class variables
    {
        using field_type = decltype(obj.[:member:]);  // splice specifiers turn meta info into code, example obj.Ammo

        if constexpr (std::is_same_v<field_type, const char *> )  // if field type is const char *
        {
            const char *text_value = obj.[:member:];  // Cache the variable value in a local variable

            if (text_value != 0 && std::string(text_value).find(search_word) != std::string::npos)
                return true;  // If pointer is not 0 and word is found
        }
        else if constexpr (std::is_same_v<field_type, std::string>)  // If the variable type is std::string
        {
            const std::string &text_value = obj.[:member:];
            
            if (text_value.find(search_word) != std::string::npos)
                return true;
        }
        // If it is int or bool, the compiler simply REMOVES this code for them. No extra checks during program execution!
    }

    return false;  // Nothing found
}
//------------------------------------------------------------------------------------------------------------





// Test
void Test()
{
    ATest test;
    test.Variable = 10;
    Set_Test(test);

    // Test 1: Weapon
    User_Profile user_profile;
    Weapon_Config sword;

    sword.Damage = 50;
    sword.Weapon_Name = "Excalibur";
    sword.Description = "A very powerful holy sword";
    sword.Is_Magic = true;

    user_profile.User_Id = 777;
    user_profile.Nick_Name = "GetBoros";
    user_profile.Status_Text = "Looking for a holy artifact";

    // Looking for the word "holy" in different objects!
    const char *target_word = "holy";

    bool is_in_sword = Search_Word_In_Object(sword, target_word);
    bool is_in_user = Search_Word_In_Object(user_profile, target_word);

    std::println("Word '{}' in sword: {}", target_word, is_in_sword );
    std::println("Word '{}' in user profile: {}", target_word, is_in_user );
}
//------------------------------------------------------------------------------------------------------------
void Test_01()
{// Example how work reflection with enums

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
