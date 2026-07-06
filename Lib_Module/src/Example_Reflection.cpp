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
    void Temp();

    int Value_00;
    int Value_01;
};
//------------------------------------------------------------------------------------------------------------
template<typename type_name> void TExample_Reflection(const type_name &object, int value)
{
    constexpr std::meta::info object_info = ^^std::remove_cvref_t<type_name>;
    constexpr std::meta::access_context access_ctx = std::meta::access_context::unchecked();

    static constexpr auto fields_data = std::define_static_array(std::meta::nonstatic_data_members_of(object_info, access_ctx) );
    static constexpr auto fields_func = std::define_static_array(std::meta::members_of(object_info, access_ctx) );

    template for(constexpr auto field_data : fields_data)
    {
        constexpr auto field_name = std::meta::identifier_of(field_data);

        std::println("Field name: {}. ", field_name, object.[:field_data:]);
    }

    template for(constexpr auto field_func : fields_func)
    {
        constexpr auto is_identifier = std::meta::has_identifier(field_func);

        if constexpr (is_identifier)
        {
            constexpr auto field_func_name = std::meta::identifier_of(field_func);  // Caching member name

            std::println("Field func name: {}. ", field_func_name);

        }
    }
 
}
//------------------------------------------------------------------------------------------------------------




// AExample_Reflection
void AExample_Reflection::Init()
{
    AObject object;
    TExample_Reflection(object, 5);
}
//------------------------------------------------------------------------------------------------------------
