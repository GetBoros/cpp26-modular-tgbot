//------------------------------------------------------------------------------------------------------------
module;
#include <iostream>
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
        SMessage message; // Created fresh on stack every loop iteration
        
        AString response = network.Get_Response(last_update_id);

        bool has_response = (response.Get_Size() > 0);

        if(has_response == true)
        {
            last_update_id = parser.Parse_Response_To_Message(response, message);
            
            bool is_valid_chat = (message.Chat.Id != 0);

            if(is_valid_chat == true)
            {
                network.Send_Message(message.Chat.Id, message.Message_Thread_Id, "Message Handled Successfully!");
            }
        }
    }
    //int last_update_id;
//
    //SMessage message;
    //ATGB_Parser parser;
    //ATGB_Network network;
//
    //last_update_id = 0;
    //
    //while(true)
    //{
    //    long long id_chat, id_chat_topic;
    //    
    //    const char *response = network.Get_Response(last_update_id);
//
    //    last_update_id = parser.Set_Response_Text(response, id_chat, id_chat_topic);
    //    if(id_chat != 0)  // Example chat_id and message
    //        network.Send_Message(id_chat, id_chat_topic, "Message Handled Successfully!");
    //}
}
//------------------------------------------------------------------------------------------------------------
