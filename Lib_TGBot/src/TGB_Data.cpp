//------------------------------------------------------------------------------------------------------------
module;

module TGB_Data;
//------------------------------------------------------------------------------------------------------------



// AString
AString::~AString()
{
    if(Is_Heap == true)
    {
        delete[] Heap_Data;
        Heap_Data = 0;
    }
}
//------------------------------------------------------------------------------------------------------------
AString::AString()
 : Is_Heap(false), Size(0), Capacity(23), Heap_Data(0)
{
    Local_Data[0] = '\0';
}
//------------------------------------------------------------------------------------------------------------
AString::AString(const char *input_data, long long input_size)
 : Is_Heap(false), Size(input_size), Capacity(23), Heap_Data(0)
{
    bool needs_heap = (Size > Capacity);

    if(needs_heap == true)
    {
        Is_Heap = true;
        Capacity = Size;
        Heap_Data = new char[Capacity + 1];
        
        Copy_Memory(Heap_Data, input_data, Size);
        Heap_Data[Size] = '\0';
    }
    else
    {
        Copy_Memory(Local_Data, input_data, Size);
        Local_Data[Size] = '\0';
    }
}
//------------------------------------------------------------------------------------------------------------
AString::AString(const AString &other)
 : Is_Heap(false), Size(other.Size), Capacity(other.Capacity), Heap_Data(0)
{
    if(other.Is_Heap == true)
    {
        Is_Heap = true;
        Heap_Data = new char[Capacity + 1];
        
        Copy_Memory(Heap_Data, other.Heap_Data, Size + 1);
    }
    else
    {
        Copy_Memory(Local_Data, other.Local_Data, Size + 1);
    }
}
//------------------------------------------------------------------------------------------------------------
AString &AString::operator=(const AString &other)
{
    bool is_same_object = (this == &other);

    if(is_same_object == true)
    {
        return *this;
    }

    if(Is_Heap == true)
    {
        delete[] Heap_Data;
        Heap_Data = 0;
    }

    Is_Heap = other.Is_Heap;
    Size = other.Size;
    Capacity = other.Capacity;

    if(other.Is_Heap == true)
    {
        Heap_Data = new char[Capacity + 1];
        
        Copy_Memory(Heap_Data, other.Heap_Data, Size + 1);
    }
    else
    {
        Copy_Memory(Local_Data, other.Local_Data, Size + 1);
    }

    return *this;
}
//------------------------------------------------------------------------------------------------------------
const char *AString::Get_C_Str() const
{
    if(Is_Heap == true)
    {
        return Heap_Data;
    }
    
    return Local_Data;
}
//------------------------------------------------------------------------------------------------------------
long long AString::Get_Size() const
{
    return Size;
}
//------------------------------------------------------------------------------------------------------------
void AString::Copy_Memory(char *destination, const char *source, long long length)
{
    for(long long i = 0; i < length; ++i)
    {
        destination[i] = source[i];
    }
}
//------------------------------------------------------------------------------------------------------------
