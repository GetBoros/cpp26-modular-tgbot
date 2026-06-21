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
    Pimpl = new SPimpl();

    Initialize();
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Network::Initialize()
{
    constexpr const int response_status_ok = 200;
    const char *env_token = std::getenv("TELEGRAM_BOT_TOKEN");
    std::string bot_token;

    bot_token = env_token;  // Set token here
    Pimpl->API_URL_GET_UPDATES = "https://api.telegram.org/bot" + bot_token + "/getUpdates";
    Pimpl->API_URL_SEND_MESSAGE = "https://api.telegram.org/bot" + bot_token + "/sendMessage";

    std::println("Bot started! Waiting for messages...");
}
//------------------------------------------------------------------------------------------------------------
AString ATGB_Network::Get_Response(int update_id) const
{
    cpr::Response response;
    cpr::Url url_target_str = cpr::Url { Pimpl->API_URL_GET_UPDATES };
    cpr::Parameters url_param = cpr::Parameters { {"timeout", "10"}, {"offset", std::to_string(update_id + 1) } };
    constexpr const int response_status_ok = 200;
    
    // 1.0. Wait response 10 sec if ok return with result
    response = cpr::Get(url_target_str, url_param);
    if(response.status_code == response_status_ok)  // Convert cpr's std::string to our fast AString and return
        return AString(response.text.c_str(), response.text.size() );
    
    // 1.1. If result status not ok print errors and return empty string
    std::println("Network Error: {}", response.status_code);
    std::this_thread::sleep_for(std::chrono::seconds(1) );

    return AString(); // Return empty AString on error
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Network::Send_Message(long long chat_id, long long message_thread_id, const char *text) const
{
    std::vector<cpr::Pair> payload_fields;
    std::string url_method_str;
    cpr::Url url_target_str;
    cpr::Response response;
    constexpr int response_status_ok = 200;
    
    // 1.0. Initialize url and build payload fields
    url_method_str = Pimpl->API_URL_SEND_MESSAGE;  // Get API url for sending msg`s
    url_target_str = cpr::Url { url_method_str };  // Build target url
    payload_fields =
    {
        {"chat_id", std::to_string(chat_id) },  // In field chat_id write chat_id
        {"text", text}  // Inf field text write message text
    };

    // 1.1. Append topic id if message targets a specific thread
    if (message_thread_id != 0)  // if topic id not 0, add him to payload
        payload_fields.push_back( {"message_thread_id", std::to_string(message_thread_id) } );

    const cpr::Payload payload_data = cpr::Payload(payload_fields.begin(), payload_fields.end() );

    // 2.0. Execute API request, send POST request to Telegram API to send message and print logs
    response = cpr::Post(url_target_str, payload_data);  
    if (response.status_code == response_status_ok)
        std::println("Message sent successfully to chat: {}", chat_id);
    else
        std::println("Failed to send message. Error: {}, Response: {}", response.status_code, response.text);
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Network::Send_Message_Reply(long long chat_id, long long message_thread_id, long long message_id, const char *text) const
{
    std::vector<cpr::Pair> payload_fields;
    std::string url_method_str;
    std::string reply_json_str;
    std::string keyboard_json_str;
    cpr::Url url_target_str;
    cpr::Response response;
    constexpr int response_status_ok = 200;

    // 1.0. Initialize url and build payload fields
    url_method_str = Pimpl->API_URL_SEND_MESSAGE;
    url_target_str = cpr::Url { url_method_str };
    payload_fields =
    {
        {"chat_id", std::to_string(chat_id) },
        {"text", text}
    };

    // 1.1. Append target topic identifier if thread context is specified
    if(message_thread_id != 0)
        payload_fields.push_back( {"message_thread_id", std::to_string(message_thread_id) } );

    // 1.2. Embed reply parameters containing parent message identifier
    if(message_id != 0)
    {
        reply_json_str = "{\"message_id\":" + std::to_string(message_id) + "}";
        payload_fields.push_back( {"reply_parameters", std::move(reply_json_str) } );
    }

    // 1.3. Embed inline keyboard with Accept and Decline buttons.
    keyboard_json_str = "{\"inline_keyboard\":[[{\"text\":\"Accept\",\"callback_data\":\"action_accept\"},{\"text\":\"Decline\",\"callback_data\":\"action_decline\"}]]}";
    payload_fields.push_back({"reply_markup", std::move(keyboard_json_str) } );  // Attach keyboard to payload.

    const cpr::Payload payload_data = cpr::Payload(payload_fields.begin(), payload_fields.end() );

    // 2.0. Execute API request, send POST request to Telegram API to send message and print logs
    response = cpr::Post(url_target_str, payload_data);
    if(response.status_code == response_status_ok)  // Output operation outcome details to logs
        std::println("Message reply sent successfully to chat: {}", chat_id);
    else
        std::println("Failed to send reply. Error: {}, Response: {}", response.status_code, response.text);
}
//------------------------------------------------------------------------------------------------------------
