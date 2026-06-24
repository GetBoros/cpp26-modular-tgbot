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

    AString Get_NBU_USD_Rate() const;
    AString Get_Response(int update_id) const;
    AString Connect(int update_id) const;
    
    void Send_Message(long long chat_id, long long message_thread_id, const char *text) const;
    void Send_Message_Reply(long long chat_id, long long message_thread_id, long long message_id, const char *text) const;
        
    bool Answer_Callback_Query(const AString &callback_query_id) const;
    void Delete_Message(long long chat_id, long long message_id);

    void Edit_Message_Text(long long chat_id, long long message_id, const AString &new_text_str);
    void Edit_Message_Reply_Markup(long long chat_id, long long message_id, const AString &markup_json_str);

private:
    struct SPimpl *Pimpl;

};
//------------------------------------------------------------------------------------------------------------
/* TASKS
    - Login to site and parse other info
    - Create custom account for bot
    - Get USD info or crypto other stuff
*/
//------------------------------------------------------------------------------------------------------------
