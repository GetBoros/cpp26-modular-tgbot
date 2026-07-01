//------------------------------------------------------------------------------------------------------------
module;

export module TGB_Bot_API;
//------------------------------------------------------------------------------------------------------------
import TGB_Data;
//------------------------------------------------------------------------------------------------------------
export class ATGB_Bot_API
{
public:
    ~ATGB_Bot_API();
    ATGB_Bot_API();

    void Initialize();

    STelegram_Event Poll_Events_Temp();  // Get data from tg bot

    void Send_Message(long long chat_id, long long message_thread_id, const char *text) const;
    void Send_Message_Reply(long long chat_id, long long message_thread_id, long long message_id, const char *text) const;
    void Send_Game_Web_App(long long chat_id, long long message_thread_id, const char *text, const char *url_str) const;
    void Answer_Callback_Query(const AString &callback_query_id) const;

    void Edit_Message_Text(long long chat_id, long long message_id, const AString &new_text_str);
    void Edit_Message_Reply_Markup(long long chat_id, long long message_id, const AString &markup_json_str);

    void Delete_Message(long long chat_id, long long message_id);

    AString Get_NBU_USD_Rate() const;  // !!! TEMP

private:
    void Set_My_Commands() const;  // Set menu in bot private chat
    void Delete_My_Commands() const;  // !!! can`t use ATemp Delete menu in bot private chat
    
    struct SPimpl *Pimpl;

};
//------------------------------------------------------------------------------------------------------------
/* TASKS
    - Login to site and parse other info
    - Create custom account for bot
    - Get USD info or crypto other stuff
*/
//------------------------------------------------------------------------------------------------------------
