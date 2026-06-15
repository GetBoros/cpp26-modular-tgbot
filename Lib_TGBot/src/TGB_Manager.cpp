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
        const char *response = network.Get_Response(last_update_id);
        last_update_id = parser.Set_Response_Text(response);
        network.Send_Message(1775156303, "Hello, World!");  // Example chat_id and message
    }
}
//------------------------------------------------------------------------------------------------------------
