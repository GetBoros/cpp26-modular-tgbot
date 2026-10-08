//------------------------------------------------------------------------------------------------------------
module;

export module Compile_Time;
//------------------------------------------------------------------------------------------------------------
import std;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
export void Compile_Time_Preview();
//------------------------------------------------------------------------------------------------------------




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




// AConstexpr_Example || Example 
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
