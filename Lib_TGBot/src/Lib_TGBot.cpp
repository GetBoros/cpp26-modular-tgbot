//------------------------------------------------------------------------------------------------------------
module;
#include <string>
#include <print>
#include <cstdlib>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
module Lib_TGBot;
//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
void Test_1()
{
    const char *env_token = std::getenv("TELEGRAM_BOT_TOKEN");
    int last_update_id = 0;  // Variable to store last read message ID
    constexpr const int response_status_ok = 200;

    std::string bot_token = env_token;  // Set token here
    std::string api_url = "https://api.telegram.org/bot" + bot_token + "/getUpdates";
    

    std::println("Bot started! Waiting for messages...");

    // Бесконечный цикл бота
    while (true)
    {
        auto url_ts = cpr::Url { api_url };  // api_url - https://api.telegram.org/bot<TOKEN>/getUpdates
        auto url_param = cpr::Parameters { {"timeout", "10"}, {"offset", std::to_string(last_update_id + 1) } };
        
        cpr::Response response = cpr::Get(url_ts, url_param);  // Long polling request to Telegram API

        if (response.status_code == response_status_ok)
        {
            try
            {
                nlohmann::json json_data = nlohmann::json::parse(response.text);  // Telegram response parsing
                
                for (const auto &item : json_data["result"])  // find msg in result arr, can be more than one
                {
                    last_update_id = item["update_id"];  // set new last update id, if need get next msg

                    std::string pretty_json = json_data.dump(4);
                    std::println("Received JSON:\n{}", pretty_json);  // for debug to see what inside

                    if (item.contains("message") && item["message"].contains("text") )  // if msg have text print text and user name
                    {
                        std::string message_text = item["message"]["text"];
                        std::string user_name = item["message"]["from"]["first_name"];
                        
                        std::println("[{}] says: {}", user_name, message_text);
                    }
                }
            }
            catch (const nlohmann::json::exception &e)
            {
                std::println("JSON Error: {}", e.what() );
            }
        }
        else
        {
            std::println("Network Error: {}", response.status_code);

            std::this_thread::sleep_for(std::chrono::seconds(1) );
        }
    }
}
//------------------------------------------------------------------------------------------------------------
void Handle_TGBot()
{
    Test_1();
}
//------------------------------------------------------------------------------------------------------------
