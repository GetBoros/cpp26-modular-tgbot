//------------------------------------------------------------------------------------------------------------
module;

export module TGB_Data;
//------------------------------------------------------------------------------------------------------------
export class AString
{

public:
    virtual ~AString();

    AString();
    AString(const char *input_data, long long input_size);
    AString(const AString &other);

    AString &operator=(const AString &other);

    const char *Get_C_Str() const;
    long long Get_Size() const;

private:
    static void Copy_Memory(char *destination, const char *source, long long length);

    bool Is_Heap;
    long long Size;
    long long Capacity;
    char *Heap_Data;
    char Local_Data[24];

};
//------------------------------------------------------------------------------------------------------------
export struct SChat
{
    long long Id;
    bool Is_Forum;

    AString Title;
    AString Type;
};
//------------------------------------------------------------------------------------------------------------
export struct SUser_Info
{
    long long Id;
    bool Is_Bot;

    AString First_Name;
};
//------------------------------------------------------------------------------------------------------------
export struct SMessage
{
    long long Message_Id;
    long long Message_Thread_Id;

    AString Text;
    SUser_Info From;
    SChat Chat;
};
//------------------------------------------------------------------------------------------------------------
export struct SCallback_Query
{
    AString Id;
    AString Data;
    SUser_Info From;
    SMessage Message;
};
//------------------------------------------------------------------------------------------------------------
export struct SUpdate
{
    long long Update_Id;

    SMessage Message;
    SCallback_Query Callback_Query;
};
//------------------------------------------------------------------------------------------------------------
export struct SExchange_Rate
{
    int R030;
    double Rate;

    AString Cc;
    AString Exchangedate;
    AString Special;
    AString Txt;
};
//------------------------------------------------------------------------------------------------------------
