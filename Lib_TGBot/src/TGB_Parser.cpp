//------------------------------------------------------------------------------------------------------------
module;
#include <print>
#include <nlohmann/json.hpp>
module TGB_Parser;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
struct SPimpl
{
    std::string Response_Text;

};
//------------------------------------------------------------------------------------------------------------




// ATGB_Parser
ATGB_Parser::ATGB_Parser()
{
    Pimpl = new SPimpl();
}
//------------------------------------------------------------------------------------------------------------
int ATGB_Parser::Set_Response_Text(const char *response_text, long long &chat_id)
{
    int last_update_id;

    last_update_id = 0;

    try
    {
        // Парсим текст. nlohmann::json живет только в этом файле!
        nlohmann::json json_data = nlohmann::json::parse(response_text);  

        for (auto const &item : json_data["result"])  
        {
            last_update_id = item["update_id"];  // set new last update id, if need get next msg
            
            std::string pretty_json = json_data.dump(4);
            std::println("Received JSON:\n{}", pretty_json);

            if (item.contains("message") && item["message"].contains("text"))  
            {
                chat_id = item["message"]["chat"]["id"];  // Get chat_id for sending messages

                std::string message_text = item["message"]["text"];  // Get message text
                std::string user_name = item["message"]["from"]["first_name"];  // Get sender's first name
                
                std::println("[{}] says: {}", user_name, message_text);
            }
        }
    }
    catch (nlohmann::json::exception const &e)
    {
        std::println("JSON Error: {}", e.what());
    }

    return last_update_id;
}
//------------------------------------------------------------------------------------------------------------
