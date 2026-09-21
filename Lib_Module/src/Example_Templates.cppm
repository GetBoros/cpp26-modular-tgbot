//------------------------------------------------------------------------------------------------------------
module;

export module Example_Templates;
//------------------------------------------------------------------------------------------------------------
import std;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
#pragma region MODULE_1  // Templates
//------------------------------------------------------------------------------------------------------------
template<typename Type_Name> [[nodiscard]] Type_Name TFunc_Example(Type_Name test_0, Type_Name test_1) noexcept
{// SImple template example

    return test_0 + test_1;
}
//------------------------------------------------------------------------------------------------------------
template<int Multiplier> [[nodiscard]] int TFunc_Example_NTTP(int value) noexcept
{// NTTP Example Non type template param

    return Multiplier * value;
}
//------------------------------------------------------------------------------------------------------------
template<typename Type_Name> class ACTAD_Example
{// CTAD Class Template Argument Deduction Example || ACTAD_Example ctad_example { 5 };

public:
    Type_Name type_name;

};
//------------------------------------------------------------------------------------------------------------
template<typename Type_Name> class std::formatter<ACTAD_Example<Type_Name> > 
{// Example how to print any class params in ACTAD_Example

public:
    // ctx - std::println("{:04d}", 42) — ctx store symbol "04d"
    constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }  // Compile time

    auto format(const ACTAD_Example<Type_Name> &ctad_example, auto &ctx) const  // Runtime
    {
        constexpr std::meta::access_context access_ctx = std::meta::access_context::unchecked();
        static constexpr auto fields_data = std::define_static_array(std::meta::nonstatic_data_members_of(^^ACTAD_Example<Type_Name>, access_ctx) );

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
#pragma endregion // MODULE_1
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
#pragma region MODULE_2  // Compile and Runtime || constexpr constinit consteval
//------------------------------------------------------------------------------------------------------------
consteval int Example_Consteval() { return 36; };
//------------------------------------------------------------------------------------------------------------
constexpr int Example_Constexpr() { return 47; };
//------------------------------------------------------------------------------------------------------------
constexpr int Example_Test(const int data)
{
    if consteval
    {
        // std::println("consteval {}", data);

        return data;
    }
    else
    {
        std::println("constexpr {}", data);

        return data - 5;
    }
}
//------------------------------------------------------------------------------------------------------------
#pragma endregion // MODULE_2
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
export void Handle_Example_Templates();
//------------------------------------------------------------------------------------------------------------
