//------------------------------------------------------------------------------------------------------------
module;

export module Lib_Module_Reflection;
//------------------------------------------------------------------------------------------------------------
export void Test();
export void Test_01();
//------------------------------------------------------------------------------------------------------------
enum class Mouse_Button
{
    LMB,
    RMB
};
//------------------------------------------------------------------------------------------------------------
class ATest
{

public:
    virtual ~ATest() = default;

    ATest() : Is_Active(true), Variable(5), Data(15LL) { };

    void Example_Func() {};

    bool Is_Active;
    int Variable;
    long long Data;

    static int Static_Variable;

};
//------------------------------------------------------------------------------------------------------------
class Weapon_Config
{
public:
    const char *Weapon_Name;
    const char *Description;
    bool Is_Magic;
    int Damage;
};
//------------------------------------------------------------------------------------------------------------
class User_Profile
{
public:
    const char *Nick_Name;
    const char *Status_Text;
    int User_Id;
};
//------------------------------------------------------------------------------------------------------------
