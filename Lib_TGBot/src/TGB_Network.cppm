//------------------------------------------------------------------------------------------------------------
module;

export module TGB_Network;
//------------------------------------------------------------------------------------------------------------
export class ATGB_Network
{
public:
    ~ATGB_Network();
    ATGB_Network();

    void Initialize();

    const char *Get_Response(int update_id) const;
    void Send_Message(long long chat_id, long long message_thread_id, const char *message_text) const;

private:
    struct SPimpl *Pimpl;
    
};
//------------------------------------------------------------------------------------------------------------
