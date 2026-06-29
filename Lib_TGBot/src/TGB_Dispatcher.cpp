//------------------------------------------------------------------------------------------------------------
module;

module TGB_Dispatcher;
//------------------------------------------------------------------------------------------------------------
import std;
import TGB_Data;
//------------------------------------------------------------------------------------------------------------




// ATGB_Dispatcher
ATGB_Dispatcher::ATGB_Dispatcher()
{

}
//------------------------------------------------------------------------------------------------------------
void ATGB_Dispatcher::Initialize()
{
    while(true)
    {
        Tick();
    }
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Dispatcher::Tick()
{
    STelegram_Event telegram_event;

    telegram_event = Bot_API.Poll_Events_Temp();  // Not async wait connect 10 sec

    if(telegram_event.Message.Message_Id != 0)
        Bot_API.Send_Game_Web_App(telegram_event.Message.Chat.Id, "Game", "https://getboros.github.io/oni-pipeline-sim/");

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
void ATGB_Dispatcher::Send_NBU_USD_Rate()
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
