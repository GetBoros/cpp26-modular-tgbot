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
constinit int Example_Constinit = 57;  // Initialization in compile time but can be changed in runtime
//------------------------------------------------------------------------------------------------------------
consteval int Example_Consteval() { return 36; };  // Pure compile time 
//------------------------------------------------------------------------------------------------------------
constexpr int Example_Constexpr() { return 47; };  // Can be compile time or Runtime
//------------------------------------------------------------------------------------------------------------
constexpr int Example_Branches(const int data)
{// Example with branches if in compile time return handled data if not throw msg with error or can be else

    if consteval
    {
        return data * 2;  // compile time
    }
    else
    {
        throw data - 1;  // runtime
    }
}
//------------------------------------------------------------------------------------------------------------
constexpr int Example_Allocate_Mem()
{
    int result;
    int *data;

    data = new int[3] {10, 20, 30 };  // alloc mem in compile time
    result = data[0] + data[1] + data[2];  // make some logic

    delete []data;  // delete if not compile time error

    return result;
}
//------------------------------------------------------------------------------------------------------------
consteval
{
    constexpr int Players_Max = 25;
    constexpr int Players_Min = 4;

    if(Players_Max <= Players_Min)
    {
        throw 36;
    }
}
//------------------------------------------------------------------------------------------------------------
class AConstexpr_Example
{
public:
    constexpr ~AConstexpr_Example() {};  // can be set constexpr by compiler auto
    constexpr AConstexpr_Example(int value) : Value(value) { Print_Message(); };  // compile or runtime
    consteval AConstexpr_Example(double value) : Data(value) {};  // only compile time

    virtual constexpr void Temp() {};

    constexpr void Print_Message();
    constexpr int Get_Value() const;  // compile or runtime
    consteval int Get_Value() { return Value + 10; };  // only compile time

    constexpr void Set_Value(int value) { Value = value; };

private:
    int Value = 0;
    int Data = 0;
    
    static inline constinit int Test = 0;

};
//------------------------------------------------------------------------------------------------------------
constexpr void AConstexpr_Example::Print_Message()
{
    if consteval {
        Value = Value + 1;
    }
    else {
        std::println("AConstexpr_Example in runtime ");
    }
}
//------------------------------------------------------------------------------------------------------------
constexpr int AConstexpr_Example::Get_Value() const
{
    return Value;
}
//------------------------------------------------------------------------------------------------------------
#pragma endregion // MODULE_2
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
export void Handle_Example_Templates();
//------------------------------------------------------------------------------------------------------------
