//------------------------------------------------------------------------------------------------------------
module;

module TGB_Manager;
//------------------------------------------------------------------------------------------------------------
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
    int last_update_id;

    ATGB_Parser parser;
    ATGB_Network network;

    last_update_id = 0;

    while(true)
    {
        SUpdate update;
        
        AString response = network.Get_Response(last_update_id);  // Not async wait connect 10 sec

        if(response.Get_Size() > 0)
        {
            last_update_id = parser.Parse_Updates(response, update);
            
            if(update.Message.Message_Id != 0)  // !!! Example reply
                network.Send_Message_Reply(update.Message.Chat.Id, update.Message.Message_Thread_Id, update.Message.Message_Id, "Message Handled Successfully!");

            if(update.Callback_Query.Id.Get_Size() > 0)
            {
                network.Answer_Callback_Query(update.Callback_Query.Id);
                network.Delete_Message(update.Callback_Query.Message.Chat.Id, update.Callback_Query.Message.Message_Id);
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------
