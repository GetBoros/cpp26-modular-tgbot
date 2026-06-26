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
    bool needs_heap;

    // 1.0. Check if heap allocation is required
    needs_heap = (Size > Capacity);
    if(needs_heap == true)
    {
        // 1.1. Allocate memory on the heap and copy data
        Is_Heap = true;
        Capacity = Size;
        Heap_Data = new char[Capacity + 1];
        
        Copy_Memory(Heap_Data, input_data, Size);
        Heap_Data[Size] = '\0';
    }
    else
    {
        // 1.2. Store data inside local stack-allocated buffer
        Copy_Memory(Local_Data, input_data, Size);
        Local_Data[Size] = '\0';
    }
}
//------------------------------------------------------------------------------------------------------------
AString::AString(const AString &other)
 : Is_Heap(false), Size(other.Size), Capacity(other.Capacity), Heap_Data(0)
{
    // 1.0. Allocate heap memory if copy source is heap-allocated
    if(other.Is_Heap == true)
    {
        Is_Heap = true;
        Heap_Data = new char[Capacity + 1];
        
        Copy_Memory(Heap_Data, other.Heap_Data, Size + 1);
    }
    else
    {
        // 1.1. Copy directly to stack buffer
        Copy_Memory(Local_Data, other.Local_Data, Size + 1);
    }
}
//------------------------------------------------------------------------------------------------------------
AString::AString(AString &&other) noexcept
 : Is_Heap(other.Is_Heap), Size(other.Size), Capacity(other.Capacity), Heap_Data(other.Heap_Data)
{
    // 1.0. Steal pointer or copy stack data based on SSO status
    if(other.Is_Heap == true)
    {
        other.Heap_Data = 0;
        other.Is_Heap = false;
        other.Size = 0;
        other.Capacity = 23;
        other.Local_Data[0] = '\0';
    }
    else
    {
        Copy_Memory(Local_Data, other.Local_Data, Size + 1);
        
        other.Size = 0;
        other.Local_Data[0] = '\0';
    }
}
//------------------------------------------------------------------------------------------------------------
AString &AString::operator=(const AString &other)
{
    bool is_same_object;
    char *new_data;

    // 1.0. Verify self-assignment to prevent memory corruption
    is_same_object = (this == &other);
    if(is_same_object == true)
    {
        return *this;
    }

    // 2.0. Allocate new heap buffer first (Strong Exception Safety)
    new_data = 0;
    if(other.Is_Heap == true)
    {
        new_data = new char[other.Capacity + 1];
        Copy_Memory(new_data, other.Heap_Data, other.Size + 1);

        if(Is_Heap == true)
        {
            delete[] Heap_Data;
        }

        Heap_Data = new_data;
    }
    else
    {
        // 2.1. Copy directly to stack if the other string is small
        if(Is_Heap == true)
        {
            delete[] Heap_Data;
            Heap_Data = 0;
        }

        Copy_Memory(Local_Data, other.Local_Data, other.Size + 1);
    }

    // 3.0. Assign state properties
    Is_Heap = other.Is_Heap;
    Size = other.Size;
    Capacity = other.Capacity;

    return *this;
}
//------------------------------------------------------------------------------------------------------------
AString &AString::operator=(AString &&other) noexcept
{
    bool is_same_object;

    // 1.0. Verify self-assignment
    is_same_object = (this == &other);
    if(is_same_object == true)
    {
        return *this;
    }

    // 2.0. Delete existing heap resources
    if(Is_Heap == true)
    {
        delete[] Heap_Data;
        Heap_Data = 0;
    }

    // 3.0. Transfer scalar state parameters
    Is_Heap = other.Is_Heap;
    Size = other.Size;
    Capacity = other.Capacity;

    // 4.0. Steal pointer or copy stack data based on SSO status
    if(other.Is_Heap == true)
    {
        Heap_Data = other.Heap_Data;
        
        other.Heap_Data = 0;
        other.Is_Heap = false;
        other.Size = 0;
        other.Capacity = 23;
        other.Local_Data[0] = '\0';
    }
    else
    {
        Heap_Data = 0;
        Copy_Memory(Local_Data, other.Local_Data, Size + 1);
        
        other.Size = 0;
        other.Local_Data[0] = '\0';
    }

    return *this;
}
//------------------------------------------------------------------------------------------------------------
void AString::Assign(const char *input_data, long long input_size)
{
    bool needs_new_heap;
    char *new_data;

    // 1.0. Check if existing capacity is sufficient
    needs_new_heap = (input_size > Capacity);
    if(needs_new_heap == true)
    {
        // 1.1. Allocate a larger heap buffer
        new_data = new char[input_size + 1];
        Copy_Memory(new_data, input_data, input_size);
        new_data[input_size] = '\0';

        if(Is_Heap == true)
        {
            delete[] Heap_Data;
        }

        Is_Heap = true;
        Capacity = input_size;
        Heap_Data = new_data;
    }
    else
    {
        // 1.2. Reuse current active buffer to prevent heap allocations
        if(Is_Heap == true)
        {
            Copy_Memory(Heap_Data, input_data, input_size);
            Heap_Data[input_size] = '\0';
        }
        else
        {
            Copy_Memory(Local_Data, input_data, input_size);
            Local_Data[input_size] = '\0';
        }
    }

    // 2.0. Set current size
    Size = input_size;
}
//------------------------------------------------------------------------------------------------------------
void AString::Clear()
{
    Size = 0;
    if(Is_Heap == true)
    {
        Heap_Data[0] = '\0';
    }
    else
    {
        Local_Data[0] = '\0';
    }
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
    long long i;

    for(i = 0; i < length; ++i)
    {
        destination[i] = source[i];
    }
}
//------------------------------------------------------------------------------------------------------------