//------------------------------------------------------------------------------------------------------------
module;
#include <meta>
#include <concepts>
#include <print>
module Lib_Module_Reflection;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
class ATest
{

public:
    virtual ~ATest() = default;

    ATest() : Is_Active(true), Variable(5), Data(15LL) { };

    void Example_Func() {};

    bool Is_Active;
    int Variable;
    long long Data;

    static int Static_Variable;

};
//------------------------------------------------------------------------------------------------------------
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




//------------------------------------------------------------------------------------------------------------
template <typename type_name>
bool search_word_in_object(const type_name &obj, const char *search_word )
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



// 
class WeaponConfig
{
public:
    const char *Weapon_Name;
    const char *Description;
    bool Is_Magic;
    int Damage;
};
//------------------------------------------------------------------------------------------------------------
class UserProfile
{
public:
    const char *Nick_Name;
    const char *Status_Text;
    int User_Id;
};
//------------------------------------------------------------------------------------------------------------
void Test()
{
    ATest test;
    test.Variable = 10;
    Set_Test(test);

    // Test 1: Weapon
    UserProfile user;
    WeaponConfig sword;

    sword.Damage = 50;
    sword.Weapon_Name = "Excalibur";
    sword.Description = "A very powerful holy sword";
    sword.Is_Magic = true;

    user.User_Id = 777;
    user.Nick_Name = "GetBoros";
    user.Status_Text = "Looking for a holy artifact";

    // Looking for the word "holy" in different objects!
    const char *target_word = "holy";

    bool is_in_sword = search_word_in_object(sword, target_word);
    bool is_in_user = search_word_in_object(user, target_word);

    std::println("Word '{}' in sword: {}", target_word, is_in_sword );
    std::println("Word '{}' in user: {}", target_word, is_in_user );
}
//------------------------------------------------------------------------------------------------------------
