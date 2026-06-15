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
    void Send_Message(unsigned long long chat_id, const char *message_text) const;

    struct SPimpl *Pimpl;
};
//------------------------------------------------------------------------------------------------------------
