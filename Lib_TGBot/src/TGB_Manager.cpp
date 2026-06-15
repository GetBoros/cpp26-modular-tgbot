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
    ATGB_Parser parser;
    ATGB_Network network;

    const char *response = network.Get_Response(0);
    
    int last_update_id = parser.Set_Response_Text(response);
    
    const char *response_next = network.Get_Response(last_update_id);

}
//------------------------------------------------------------------------------------------------------------
