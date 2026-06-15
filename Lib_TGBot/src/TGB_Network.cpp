//------------------------------------------------------------------------------------------------------------
module;
#include <print>
#include <cpr/cpr.h>
module TGB_Network;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
struct SPimpl
{
    std::string Last_Response;
    std::string API_URL;
};
//------------------------------------------------------------------------------------------------------------



// ATGB_Network
ATGB_Network::ATGB_Network()
{
    Pimpl = new SPimpl();
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Network::Initialize()
{
    const char *env_token = std::getenv("TELEGRAM_BOT_TOKEN");
    constexpr const int response_status_ok = 200;

    std::string bot_token = env_token;  // Set token here
    Pimpl->API_URL = "https://api.telegram.org/bot" + bot_token + "/getUpdates";
    

    std::println("Bot started! Waiting for messages...");
}
//------------------------------------------------------------------------------------------------------------
const char *ATGB_Network::Get_Response(int update_id) const
{
    constexpr const int response_status_ok = 200;

    while (true)
    {
        auto url_ts = cpr::Url { Pimpl->API_URL };  // api_url - https://api.telegram.org/bot<TOKEN>/getUpdates
        auto url_param = cpr::Parameters { {"timeout", "10"}, {"offset", std::to_string(update_id + 1) } };
        
        cpr::Response response = cpr::Get(url_ts, url_param);  // Long polling request to Telegram API

        if (response.status_code == response_status_ok)
        {
            Pimpl->Last_Response = response.text;

            return Pimpl->Last_Response.c_str();
        }
        else
        {
            std::println("Network Error: {}", response.status_code);

            std::this_thread::sleep_for(std::chrono::seconds(1) );
        }
    }
}
//------------------------------------------------------------------------------------------------------------
