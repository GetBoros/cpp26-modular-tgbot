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
    int Variable;

};
//------------------------------------------------------------------------------------------------------------
template <typename type_name> void Set_Test(const type_name &object)
{
   // Forwards
    constexpr std::meta::info meta_info = ^^type_name;

    // Init
    // 'static constexpr' guarantees a stable compile-time address for the internal range iterator of 'template for'
    static constexpr auto members = std::define_static_array( std::meta::nonstatic_data_members_of( meta_info, std::meta::access_context::unchecked() ) );

    // Interaction
    template for(constexpr auto member : members)
    {
        // Forwards
        constexpr auto member_name = std::meta::identifier_of(member); // Caching member name

        // Interaction
        std::println("Field: {}, Value: {}", member_name, object.[:member:]);
    }
}
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
template <typename type_name>
bool search_word_in_object( const type_name &obj, const char *search_word )
{
    // Cache meta-information about the type at compile time.
    // Zero runtime cost.
    constexpr std::meta::info meta_info = ^^type_name;

    // Iterate through all class variables
    template for ( constexpr auto member : std::define_static_array( std::meta::nonstatic_data_members_of( meta_info, std::meta::access_context::unchecked( ) ) ) )
    {
        // Get the type of the current variable ( int, bool, const char*, etc. )
        using FieldType = decltype( obj.[:member:] );

        // Magic: if the variable type is const char*
        if constexpr ( std::is_same_v<FieldType, const char *> )
        {
            // Cache the variable value in a local variable
            const char *text_value = obj.[:member:];

            // If pointer is not 0 and word is found
            if ( text_value != 0 && std::string( text_value ).find( search_word ) != std::string::npos )
            {
                return true; // Found!
            }
        }
        // If the variable type is std::string
        else if constexpr ( std::is_same_v<FieldType, std::string> )
        {
            const std::string &text_value = obj.[:member:];
            
            if ( text_value.find( search_word ) != std::string::npos )
            {
                return true;
            }
        }
        // If it is int or bool, the compiler simply REMOVES this code for them.
        // No extra checks during program execution!
    }

    return false; // Nothing found
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

    std::println( "Word '{}' in sword: {}", target_word, is_in_sword );
    std::println( "Word '{}' in user: {}", target_word, is_in_user );
}
//------------------------------------------------------------------------------------------------------------
