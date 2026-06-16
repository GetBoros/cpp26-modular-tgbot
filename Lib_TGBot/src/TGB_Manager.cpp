//------------------------------------------------------------------------------------------------------------
module;

module TGB_Manager;
//------------------------------------------------------------------------------------------------------------
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
        long long id_chat;
        const char *response = network.Get_Response(last_update_id);

        last_update_id = parser.Set_Response_Text(response, id_chat);
        if(id_chat != 0)
            network.Send_Message(id_chat, "Message Handled Successfully!");  // Example chat_id and message
    }
}
//------------------------------------------------------------------------------------------------------------
