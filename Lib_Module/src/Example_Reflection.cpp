//------------------------------------------------------------------------------------------------------------
module;
#include <meta>
#include <print>
module Example_Reflection;
//------------------------------------------------------------------------------------------------------------
// import std;



//------------------------------------------------------------------------------------------------------------
class AObject
{
    int Value_00;
    int Value_01;
};
//------------------------------------------------------------------------------------------------------------
template<typename type_name> void TExample_Reflection(const type_name &object, int value)
{
    constexpr std::meta::info object_info = ^^std::remove_cvref_t<type_name>;
    constexpr std::meta::access_context access_ctx = std::meta::access_context::unchecked();

    static constexpr auto fields = std::define_static_array(std::meta::nonstatic_data_members_of(object_info, access_ctx) );
 
}
//------------------------------------------------------------------------------------------------------------




// AExample_Reflection
void AExample_Reflection::Init()
{
    AObject object;
    TExample_Reflection(object, 5);
}
//------------------------------------------------------------------------------------------------------------
