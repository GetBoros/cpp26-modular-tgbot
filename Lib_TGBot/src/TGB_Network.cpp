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
    std::string API_URL_GET_UPDATES;
    std::string API_URL_SEND_MESSAGE;
};
//------------------------------------------------------------------------------------------------------------



// ATGB_Network
ATGB_Network::~ATGB_Network()
{
    delete Pimpl;
}
//------------------------------------------------------------------------------------------------------------
ATGB_Network::ATGB_Network()
{
    Initialize();
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Network::Initialize()
{
    const char *env_token = std::getenv("TELEGRAM_BOT_TOKEN");
    constexpr const int response_status_ok = 200;
    std::string bot_token;

    bot_token = env_token;  // Set token here
    Pimpl = new SPimpl();
    Pimpl->API_URL_GET_UPDATES = "https://api.telegram.org/bot" + bot_token + "/getUpdates";
    Pimpl->API_URL_SEND_MESSAGE = "https://api.telegram.org/bot" + bot_token + "/sendMessage";

    std::println("Bot started! Waiting for messages...");
}
//------------------------------------------------------------------------------------------------------------
const char *ATGB_Network::Get_Response(int update_id) const
{
    constexpr const int response_status_ok = 200;

    auto url_ts = cpr::Url { Pimpl->API_URL_GET_UPDATES };  // api_url - https://api.telegram.org/bot<TOKEN>/getUpdates
    auto url_param = cpr::Parameters { {"timeout", "10"}, {"offset", std::to_string(update_id + 1) } };
    
    cpr::Response response = cpr::Get(url_ts, url_param);  // Long polling request to Telegram API

    if (response.status_code == response_status_ok)
    {
        Pimpl->Last_Response = response.text;  // Save last response for parser

        return Pimpl->Last_Response.c_str();  // Return response text as C-string for parser
    }
    else
    {
        std::println("Network Error: {}", response.status_code);

        std::this_thread::sleep_for(std::chrono::seconds(1) );
    }

    return 0;
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Network::Send_Message(long long chat_id, long long message_thread_id, const char *message_text) const
{
    constexpr const int response_status_ok = 200;
    const std::string method_url = Pimpl->API_URL_SEND_MESSAGE;  // api_url - https://api.telegram.org/bot<TOKEN>/sendMessage

    auto url_ts = cpr::Url { method_url };
    
    // Form the body of the POST request (Payload) || Send msg to id chat
    // auto payload_data = cpr::Payload { {"chat_id", std::to_string(chat_id) }, {"text", message_text} };
    std::vector<cpr::Pair> payload_fields = {
        {"chat_id", std::to_string(chat_id) },
        {"text", message_text}
    };

    // Если передан ID топика, добавляем его в Payload
    if (message_thread_id != 0)
    {
        payload_fields.push_back({"message_thread_id", std::to_string(message_thread_id)} );
    }

    auto payload_data = cpr::Payload(payload_fields.begin(), payload_fields.end() );

    cpr::Response response = cpr::Post(url_ts, payload_data);  // Send POST request to Telegram API to send message

    if (response.status_code == response_status_ok)
        std::println("Message sent successfully to chat: {}", chat_id);
    else
        std::println("Failed to send message. Error: {}, Response: {}", response.status_code, response.text);
}
//------------------------------------------------------------------------------------------------------------
