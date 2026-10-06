//------------------------------------------------------------------------------------------------------------
module;

export module Concepts;
//------------------------------------------------------------------------------------------------------------
import std;
//------------------------------------------------------------------------------------------------------------
template <typename Type> concept Addable = requires (Type type_a, Type type_b)
{
    requires sizeof(Type) <= 64;  // object size good for cash line L1

    type_a.X;  // Must have var X
    { type_a.Get_X() } noexcept -> std::convertible_to<int>;  // if func return int

    type_a + type_b;  // require operate+ is overrided - true else false
};
//------------------------------------------------------------------------------------------------------------
template <typename Type> concept Addable_Advanced = Addable<Type> && requires(Type type)
{// Example Subsumption

    { type.Get_Y() } noexcept -> std::convertible_to<int>;
};
//------------------------------------------------------------------------------------------------------------
template <typename Type> concept Stringable = std::convertible_to<Type, std::string_view>;
//------------------------------------------------------------------------------------------------------------
template <typename Type> concept Numeric = std::integral<Type> || std::floating_point<Type>;
//------------------------------------------------------------------------------------------------------------
template <Numeric Type> Type Example_Add(Type a, Type b)
{
    return a + b;
}
//------------------------------------------------------------------------------------------------------------
template<Stringable Type> std::string_view Example_String_View(Type type, std::size_t offset)
{
    std::string_view temp = type;

    return temp.substr(offset);
}
//------------------------------------------------------------------------------------------------------------
template<Addable Type> Type Example_Sum(Type type_a, Type type_b)
{
    return type_a + type_b;
}
//------------------------------------------------------------------------------------------------------------
template<Addable_Advanced Type> Type Example_Sum(Type type_a, Type type_b)
{
    type_a.Get_Y();

    std::println("Addable_Advanced called!");

    return type_a + type_b;
}
//------------------------------------------------------------------------------------------------------------
template <Numeric... Type> auto Example_Sum_All(Type... types)
{
    return (types + ...);
}
//------------------------------------------------------------------------------------------------------------
template <typename Type> void Example_Print_Info(const Type &type)
{
    if constexpr (requires { type.To_String(); } )
        std::println("Print Info result {}", type.To_String() );
    else
        std::println("Type doenst have func {}");
}
//------------------------------------------------------------------------------------------------------------
template <typename Type_A, typename Type_B> bool Example_Compare(Type_A type_a, Type_B type_b) requires requires { type_a == type_b; }
{// Example Trailing requires-clause

    return type_a == type_b;
}
//------------------------------------------------------------------------------------------------------------
template <std::ranges::range Type> void Example_Print_Range(const Type &container)
{// std::vector<int>, std::array<int, 5>, std::string_view

    std::print("[ ");

    for (const auto& item : container)
        std::print("{} ", item);

    std::println("]");
}
//------------------------------------------------------------------------------------------------------------
struct APoint
{
    APoint operator+(APoint point) const
    {
        return APoint { X + point.X, Y + point.Y };
    }

    int Get_X() noexcept { return X; };
    int Get_Y() noexcept { return Y; };
    std::string To_String() const { return std::format("Point(X: {}, Y: {})", X, Y); };

    int X;
    int Y;
};
//------------------------------------------------------------------------------------------------------------
export void Concepts_Preview();
//------------------------------------------------------------------------------------------------------------
