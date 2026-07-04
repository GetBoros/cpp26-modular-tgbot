//------------------------------------------------------------------------------------------------------------
module;
#include <meta>
#include <print>
module Example_Reflection;
//------------------------------------------------------------------------------------------------------------
// import std;



//------------------------------------------------------------------------------------------------------------
struct SObject
{
    int Value;
};
//------------------------------------------------------------------------------------------------------------
template<typename type_name> void TExample_Reflection(const type_name &object, int value)
{
    constexpr std::meta::info object_info = ^^object;
    constexpr std::meta::access_context access_ctx = std::meta::access_context::unchecked();

    static constexpr auto fields = std::define_static_array(std::meta::nonstatic_data_members_of(object_info, access_ctx) );
    // static constexpr auto fields = std::define_static_array(std::meta::nonstatic_data_members_of(meta_info, access_ctx) );

    // std::println("Hello {}", field_name);


}
//------------------------------------------------------------------------------------------------------------




// AExample_Reflection
void AExample_Reflection::Init()
{
    SObject object;
    // TExample_Reflection(object, 5);
}
//------------------------------------------------------------------------------------------------------------
