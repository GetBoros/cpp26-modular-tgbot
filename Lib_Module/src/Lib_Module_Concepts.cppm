//------------------------------------------------------------------------------------------------------------
module;

export module Lib_Module_Concepts;
//------------------------------------------------------------------------------------------------------------
import std;
//------------------------------------------------------------------------------------------------------------
// HELP
/*
    1. Concepts (e.g., std::integral, CIs_Large)
        - Define compile-time constraints for templates/auto parameters.
        - Act as "face control" for functions: void Func(std::integral auto value);
        - Syntax: template<typename T> concept Concept_Name = [compile-time boolean condition];

    2. Type Traits (e.g., std::is_integral_v, std::remove_pointer_t)
        - Compile-time tools to inspect or modify types.
        - Suffix "_v" (Value): Returns a boolean (true/false). 
          Example: std::is_pointer_v<T> asks "Is T a pointer?".
        - Suffix "_t" (Type): Returns a modified type. 
          Example: std::remove_pointer_t<int*> returns 'int'.

    3. decltype(variable_name)
        - Stands for "Declared Type". 
        - Extracts the type of an existing variable at compile-time.
        - Useful when you have a variable but need its type for a Type Trait.

    4. if constexpr (...)
        - A compile-time branch. 
        - The compiler evaluates the condition. If false, the code inside the block 
          is completely discarded and NOT compiled into the final binary.
        - Perfect for writing generic code without compilation errors.
*/
//------------------------------------------------------------------------------------------------------------
export template<typename Type> concept CIs_Large = sizeof(Type) > 4;
//------------------------------------------------------------------------------------------------------------
export template<typename Type> concept CIs_Interactive = requires (Type instance)
{
    instance.Example_Func();
};
//------------------------------------------------------------------------------------------------------------
export template <typename Type> concept CIs_Iterable = requires(Type instance)
{
    std::begin(instance);
    std::end(instance);
};
//------------------------------------------------------------------------------------------------------------
template <typename type_param> constexpr std::string_view Get_Type_Name()  // example if type_param double
{
    unsigned int prefix_length;
    unsigned int suffix_length;
    unsigned int type_param_length;

    // "constexpr std::string_view Get_Type_Name() [with type_param = double; std::string_view = ...]"
    std::string_view func_signature = __PRETTY_FUNCTION__;
    constexpr std::string_view search_target = "type_param = ";
    
    prefix_length = func_signature.find(search_target);
    if (prefix_length == std::string_view::npos) 
        return "unknown";
    
    prefix_length += search_target.length();  // ptr at "d" in double
    suffix_length = func_signature.find_first_of(";]", prefix_length);  // find ";" or "]" from prefix_length - d
    type_param_length = suffix_length - prefix_length;
    if (suffix_length == std::string_view::npos)
        return "unknown";
    
    return func_signature.substr(prefix_length, type_param_length);  // copy from (d, ; - d )
}
//------------------------------------------------------------------------------------------------------------
export namespace NLib_Module_Concepts
{
    void Print_Smart(auto value);  // "example std::is_pointer_v"
    void Print_Analize(auto is_input_value);  // "example vectors"
    auto Print_Multiplied_Value(auto value);  // "example std::remove_pointer_t and is_pointer_v"
    void Compare_Types(auto type_first, auto type_second);  // "example std::is_same_v"
    void Compare_Derived(auto type_first, auto type_second);  // "example std::is_base_of_v"
    void Print_Custom_Concept(CIs_Large auto is_value_large);
    void Print_If_Interactive(CIs_Interactive auto is_interactive);

};
//------------------------------------------------------------------------------------------------------------




// NLib_Module_Concepts
void NLib_Module_Concepts::Print_Smart(auto value)
{
    if constexpr (std::is_pointer_v<decltype(value)>)
        std::println("It is a pointer! Value inside: {}", *value);
    else
        std::println("It is a regular value: {}", value);

}
//------------------------------------------------------------------------------------------------------------
void NLib_Module_Concepts::Print_Analize(auto is_input_val)
{
    int local_increment = 1;

    using raw_type = decltype(is_input_val);

    if constexpr (std::is_pointer_v<raw_type>)
    {
        using inner_type = std::remove_pointer_t<raw_type>;

        std::println("Processing a pointer! Pointed value: {}", *is_input_val);
        std::println("The underlying raw type was: {}", Get_Type_Name<inner_type>());
    }
    else if constexpr (CIs_Iterable<raw_type>)
    {
        std::print("Processing an iterable container: [ ");

        for (const auto& element_val : is_input_val)
            std::print("{} ", element_val);

        std::println("]");
    }
    else
        std::println("Processing a basic value: {}", is_input_val);
}
//------------------------------------------------------------------------------------------------------------
auto NLib_Module_Concepts::Print_Multiplied_Value(auto value)
{
    std::remove_pointer_t<decltype(value)> result_value;

    if constexpr (std::is_pointer_v<decltype(value)>)
        result_value = *value;  // if ptr use *
    else
        result_value = value;  // just copy

    result_value = result_value * 2;
    std::println("Muliplied value is {} done",result_value);

    return result_value;
}
//------------------------------------------------------------------------------------------------------------
void NLib_Module_Concepts::Compare_Types(auto type_first, auto type_second)
{
    if constexpr (std::is_same_v<decltype(type_first), decltype(type_second)>)
        std::println("The types is same");
    else
        std::println("The types is not same");
}
//------------------------------------------------------------------------------------------------------------
void NLib_Module_Concepts::Compare_Derived(auto type_base, auto type_derived)
{
    if constexpr (std::is_base_of_v<decltype(type_base), decltype(type_derived)>)
        std::println("The types is derived");
    else
        std::println("The types is not derived");
}
//------------------------------------------------------------------------------------------------------------
void NLib_Module_Concepts::Print_Custom_Concept(CIs_Large auto is_value_large)
{
    constexpr std::string_view type_name = Get_Type_Name<decltype(is_value_large)>();

    std::println("The type '{}' is > 4 bytes! Value: {}", type_name, is_value_large);
}
//------------------------------------------------------------------------------------------------------------
void NLib_Module_Concepts::Print_If_Interactive(CIs_Interactive auto is_interactive)
{
    // is_interactive.Example_Func();

    std::println("Class Has Example_Func");
}
//------------------------------------------------------------------------------------------------------------
