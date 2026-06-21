//------------------------------------------------------------------------------------------------------------
module;

export module TGB_Network;
//------------------------------------------------------------------------------------------------------------
import TGB_Data;
//------------------------------------------------------------------------------------------------------------
export class ATGB_Network
{
public:
    ~ATGB_Network();
    ATGB_Network();

    void Initialize();

    AString Get_Response(int update_id) const;

    void Send_Message(long long chat_id, long long message_thread_id, const char *text) const;
    void Send_Message_Reply(long long chat_id, long long message_thread_id, long long message_id, const char *text) const;

private:
    struct SPimpl *Pimpl;

};
//------------------------------------------------------------------------------------------------------------
