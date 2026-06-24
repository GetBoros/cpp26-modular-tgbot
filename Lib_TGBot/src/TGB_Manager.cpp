//------------------------------------------------------------------------------------------------------------
module;

module TGB_Manager;
//------------------------------------------------------------------------------------------------------------
import std;
import TGB_Data;
import TGB_Parser;
import TGB_Network;
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
    int last_update_id;
    double usd;
    std::string msg_send;
    SExchange_Rate exchange_rate;

    AString response;
    ATGB_Parser parser;
    ATGB_Network network;

    last_update_id = 0;
    response = network.Get_NBU_USD_Rate();
    usd = parser.Parse_NBU_USD_Rate(response, exchange_rate);
    msg_send = std::format("Currency: {} - {:.4f}, {}", exchange_rate.Cc.Get_C_Str(), exchange_rate.Rate, exchange_rate.Exchangedate.Get_C_Str() );

    while(true)
    {
        SUpdate update;

        response = network.Connect(last_update_id);  // Not async wait connect 10 sec

        if(response.Get_Size() > 0)
        {
            last_update_id = parser.Parse_Updates(response, update);

            if(update.Message.Message_Id != 0)  // !!! Example reply
                network.Send_Message_Reply(update.Message.Chat.Id, update.Message.Message_Thread_Id, update.Message.Message_Id, msg_send.c_str() );

            if(update.Callback_Query.Id.Get_Size() > 0)
            {
                int size = sizeof("Handled");
                AString text_count = AString("Handled", size);

                network.Answer_Callback_Query(update.Callback_Query.Id);
                network.Edit_Message_Reply_Markup(update.Callback_Query.Message.Chat.Id, update.Callback_Query.Message.Message_Id, text_count);
                // network.Delete_Message(update.Callback_Query.Message.Chat.Id, update.Callback_Query.Message.Message_Id);
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------
