//------------------------------------------------------------------------------------------------------------
module;

module TGB_Manager;
//------------------------------------------------------------------------------------------------------------
import std;
import TGB_Data;
import TGB_Bot_API;
import TGB_Deserializer;
//------------------------------------------------------------------------------------------------------------




// ATGB_Manager
ATGB_Manager::ATGB_Manager()
{

}
//------------------------------------------------------------------------------------------------------------
void ATGB_Manager::Initialize()
{
    Tick();
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Manager::Tick()
{
    int last_processed_event_id;
    double usd;
    std::string formatted_rate_msg;
    SCurrency_Rate currency_rate;

    AString response;
    ATGB_Deserializer deserializer;
    ATGB_Bot_API bot_api;

    last_processed_event_id = 0;
    response = bot_api.Get_NBU_USD_Rate();
    deserializer.Deserialize_NBU_USD_Rate(response, currency_rate);
    usd = currency_rate.Rate;
    formatted_rate_msg = std::format("Currency: {} - {:.4f}, {}", currency_rate.Cc.Get_C_Str(), currency_rate.Rate, currency_rate.Exchangedate.Get_C_Str() );
    
    while(true)
    {
        STelegram_Event telegram_event;

        // 1.0.
        bot_api.Poll_Events(last_processed_event_id, response);  // Not async wait connect 10 sec
        if(response.Get_Size() <= 0)
            return;  // if no connection or else problem - quit

        deserializer.Deserialize_Event(response, telegram_event);  // deserialize response to tg event struct
        last_processed_event_id = telegram_event.Update_Id;  // ready to get new msg, callback.

        // !!! 1.0 TEST
        // if(telegram_event.Message.Message_Id != 0)  // !!! Example reply
            // bot_api.Send_Message_Reply(telegram_event.Message.Chat.Id, telegram_event.Message.Message_Thread_Id, telegram_event.Message.Message_Id, formatted_rate_msg.c_str() );

        // // !!! 2.0. TEST
        // if(telegram_event.Callback_Query.Id.Get_Size() > 0)
        // {
            // int size = sizeof("Handled");
            // AString text_count = AString("Handled", size);

            // bot_api.Answer_Callback_Query(telegram_event.Callback_Query.Id);
            // bot_api.Edit_Message_Reply_Markup(telegram_event.Callback_Query.Message.Chat.Id, telegram_event.Callback_Query.Message.Message_Id, text_count);
            // // bot_api.Delete_Message(telegram_event.Callback_Query.Message.Chat.Id, telegram_event.Callback_Query.Message.Message_Id);
        // }
    }
}
//------------------------------------------------------------------------------------------------------------
