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
    long long Message_Thread_Id;
    AString Text;
    
    SUser_Info From;
    SChat Chat;
};
//------------------------------------------------------------------------------------------------------------
