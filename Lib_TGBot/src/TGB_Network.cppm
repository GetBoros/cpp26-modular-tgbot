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

    AString Get_Update_Response(int update_id) const;  // Get data from tg bot

    AString Get_NBU_USD_Rate() const;
    
    void Send_Message(long long chat_id, long long message_thread_id, const char *text) const;
    void Send_Message_Reply(long long chat_id, long long message_thread_id, long long message_id, const char *text) const;
        
    void Answer_Callback_Query(const AString &callback_query_id) const;
    void Delete_Message(long long chat_id, long long message_id);

    void Edit_Message_Text(long long chat_id, long long message_id, const AString &new_text_str);
    void Edit_Message_Reply_Markup(long long chat_id, long long message_id, const AString &markup_json_str);

    void Send_Game_Web_App(long long chat_id, const char *text, const char *url_str) const;

private:
    void Set_My_Commands() const;  // Set menu in bot private chat
    void Delete_My_Commands() const;  // Delete menu in bot private chat

    struct SPimpl *Pimpl;

};
//------------------------------------------------------------------------------------------------------------
/* TASKS
    - Login to site and parse other info
    - Create custom account for bot
    - Get USD info or crypto other stuff
*/
//------------------------------------------------------------------------------------------------------------
