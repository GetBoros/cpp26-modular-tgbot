//------------------------------------------------------------------------------------------------------------
module;

module TGB_Manager;
//------------------------------------------------------------------------------------------------------------
import TGB_Data;
import TGB_Deserializer;
//------------------------------------------------------------------------------------------------------------




// ATGB_Manager
ATGB_Manager::ATGB_Manager()
{

}
//------------------------------------------------------------------------------------------------------------
void ATGB_Manager::Initialize()
{
    bool is_running;

    // 1.0. Execute the main event dispatcher loop
    is_running = true;
    do
    {
        Tick();
    } 
    while(is_running == true);  // if no connection or else problem - quit
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Manager::Tick()
{
    STelegram_Event telegram_event;

    // 1.0.
    telegram_event = Bot_API.Poll_Events_Temp();  // Not async wait connect 10 sec
    // Bot_API.Poll_Events(Lst_Processed_Event_Id, Response);  // Not async wait connect 10 sec

    // Deserializer.Deserialize_Event(Response, telegram_event);  // deserialize Response to tg event struct

    if(telegram_event.Message.Message_Id != 0)
        std::println("Text {} ", telegram_event.Message.Text.Get_C_Str() );


    // Lst_Processed_Event_Id = telegram_event.Update_Id;  // ready to get new msg, callback.

    // !!! 1.0 TEST
    // if(telegram_event.Message.Message_Id != 0)  // !!! Example reply
        // Bot_API.Send_Message_Reply(telegram_event.Message.Chat.Id, telegram_event.Message.Message_Thread_Id, telegram_event.Message.Message_Id, formatted_rate_msg.c_str() );

    // // !!! 2.0. TEST
    // if(telegram_event.Callback_Query.Id.Get_Size() > 0)
    // {
        // int size = sizeof("Handled");
        // AString text_count = AString("Handled", size);

        // Bot_API.Answer_Callback_Query(telegram_event.Callback_Query.Id);
        // Bot_API.Edit_Message_Reply_Markup(telegram_event.Callback_Query.Message.Chat.Id, telegram_event.Callback_Query.Message.Message_Id, text_count);
        // // Bot_API.Delete_Message(telegram_event.Callback_Query.Message.Chat.Id, telegram_event.Callback_Query.Message.Message_Id);
    // }
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Manager::Send_NBU_USD_Rate()
{
    // double usd;
    // std::string formatted_rate_msg;
    // SCurrency_Rate currency_rate;

    // Response = Bot_API.Get_NBU_USD_Rate();
    // Deserializer.Deserialize_NBU_USD_Rate(Response, currency_rate);
    // usd = currency_rate.Rate;
    // formatted_rate_msg = std::format("Currency: {} - {:.4f}, {}", currency_rate.Cc.Get_C_Str(), currency_rate.Rate, currency_rate.Exchangedate.Get_C_Str() );
}
//------------------------------------------------------------------------------------------------------------
