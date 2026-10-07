//------------------------------------------------------------------------------------------------------------
module;

export module Templates;
//------------------------------------------------------------------------------------------------------------
import std;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
export void Templates_Preview();
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
template<typename Type> [[nodiscard]] Type TSum(Type type_0, Type type_1) noexcept  // SImple template example
{
    return type_0 + type_1;
}
//------------------------------------------------------------------------------------------------------------
template<int multiplier> [[nodiscard]] int TMultiply_Value(int value) noexcept  // NTTP Example Non type template param
{
    return multiplier * value;
}
//------------------------------------------------------------------------------------------------------------
template<typename Type> class ACTAD_Example
{// CTAD Class Template Argument Deduction Example || ACTAD_Example ctad_example { 5 };

public:
    Type type_name;

};
//------------------------------------------------------------------------------------------------------------
template<typename Type> class std::formatter<ACTAD_Example<Type> >  // Example how to print any class params in ACTAD_Example
{

public:
    // ctx - std::println("{:04d}", 42) — ctx store symbol "04d"
    constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }  // Compile time

    auto format(const ACTAD_Example<Type> &ctad_example, auto &ctx) const  // Runtime
    {
        constexpr std::meta::access_context access_ctx = std::meta::access_context::unchecked();
        static constexpr auto fields_data = std::define_static_array(std::meta::nonstatic_data_members_of(^^ACTAD_Example<Type>, access_ctx) );

        auto out = ctx.out();

        template for (constexpr auto field :  fields_data)
        {
            constexpr auto field_type_name = std::meta::display_string_of(std::meta::type_of(field) );
            constexpr auto field_var_name = std::meta::identifier_of(field);

            std::format_to(out, " [ {} {} = {} ] ", field_type_name, field_var_name, ctad_example.[:field:]);
        }
        return out;
    }
};
//------------------------------------------------------------------------------------------------------------
