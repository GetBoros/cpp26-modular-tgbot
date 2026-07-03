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
    AString(AString &&other) noexcept;

    AString &operator=(const AString &other);
    AString &operator=(AString &&other) noexcept;

    void Assign(const char *input_data, long long input_size);
    void Clear();

    const char *Get_C_Str() const;
    long long Get_Size() const;

private:
    bool Is_Heap;
    long long Size;
    long long Capacity;
    char *Heap_Data;
    char Local_Data[24];

    static void Copy_Memory(char *destination, const char *source, long long length);

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
export struct SReply_Markup
{
    AString Inline_Keyboard;
};
//------------------------------------------------------------------------------------------------------------
export struct SUser_Info
{
    bool Is_Bot;
    long long Id;

    AString First_Name;
    AString Language_Code;
    AString Username;

};
//------------------------------------------------------------------------------------------------------------
export struct SEntities
{
    AString Type;
    
};
//------------------------------------------------------------------------------------------------------------
export struct SMessage
{
    bool Is_Topic_Message;
    long long Message_Id;
    long long Message_Thread_Id;

    AString Text;
    SUser_Info From;
    SChat Chat;
    SReply_Markup Reply_Markup;
    SEntities Entities;

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
export struct STelegram_Event
{
    long long Update_Id;

    SMessage Message;
    SCallback_Query Callback_Query;

};
//------------------------------------------------------------------------------------------------------------
export struct SCurrency_Rate
{
    int R030;
    double Rate;

    AString Cc;
    AString Exchangedate;
    AString Special;
    AString Txt;

};
//------------------------------------------------------------------------------------------------------------
