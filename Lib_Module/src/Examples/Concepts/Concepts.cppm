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
template <typename Type> concept Stringable = std::convertible_to<Type, std::string_view>;
//------------------------------------------------------------------------------------------------------------
template <typename Type> concept Numeric = std::integral<Type>;
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
struct APoint
{
    APoint operator+(APoint point) const
    {
        return APoint { X + point.X, Y + point.Y };
    }

    int Get_X() noexcept { return X; };

    int X;
    int Y;
};
//------------------------------------------------------------------------------------------------------------
export void Test();
//------------------------------------------------------------------------------------------------------------
