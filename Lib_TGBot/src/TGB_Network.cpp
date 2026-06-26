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
    std::string API_URL_EDIT_MESSAGE_TEXT;
    std::string API_URL_EDIT_MESSAGE_REPLY_MARKUP;
    std::string API_URL_DELETE_MESSAGE;
    std::string API_URL_ANSWER_CALLBACK_QUERY;
    std::string API_URL_SET_MY_COMMANDS;
    std::string API_URL_DELETE_MY_COMMANDS;

    cpr::Session Polling_Session;
    cpr::Session Response_Session;
};
//------------------------------------------------------------------------------------------------------------



// ATGB_Network
ATGB_Network::~ATGB_Network()
{
    Delete_My_Commands();

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
    Pimpl->API_URL_EDIT_MESSAGE_TEXT = "https://api.telegram.org/bot" + bot_token + "/editMessageText";
    Pimpl->API_URL_EDIT_MESSAGE_REPLY_MARKUP = "https://api.telegram.org/bot" + bot_token + "/editMessageReplyMarkup";
    Pimpl->API_URL_DELETE_MESSAGE = "https://api.telegram.org/bot" + bot_token + "/deleteMessage";
    Pimpl->API_URL_ANSWER_CALLBACK_QUERY = "https://api.telegram.org/bot" + bot_token + "/answerCallbackQuery";
    Pimpl->API_URL_SET_MY_COMMANDS = "https://api.telegram.org/bot" + bot_token + "/setMyCommands";
    Pimpl->API_URL_DELETE_MY_COMMANDS = "https://api.telegram.org/bot" + bot_token + "/deleteMyCommands";

    Set_My_Commands();

    std::println("Bot started! Waiting for messages...");
}
//------------------------------------------------------------------------------------------------------------
AString ATGB_Network::Get_Update_Response(int update_id) const
{
    cpr::Url url;
    cpr::Parameters url_param;
    cpr::Response response;
    constexpr int response_status_ok = 200;

    // 1.0. Configure request targets and parameters
    url = cpr::Url { Pimpl->API_URL_GET_UPDATES };
    url_param = cpr::Parameters{ {"timeout", "10"}, {"offset", std::to_string(update_id + 1) } };

    // 1.1. Update session settings
    Pimpl->Polling_Session.SetUrl(url);
    Pimpl->Polling_Session.SetParameters(url_param);

    // 1.2. Execute persistent request handle response results
    response = Pimpl->Polling_Session.Get();
    if(response.status_code == response_status_ok)
        return AString(response.text.c_str(), static_cast<long long>(response.text.size() ) );

    // 2.1. Handle failures and cooldown
    std::println("Network Error: {}", response.status_code);
    std::this_thread::sleep_for(std::chrono::seconds(1) );

    return AString();
}
//------------------------------------------------------------------------------------------------------------
AString ATGB_Network::Get_NBU_USD_Rate() const
{
    cpr::Url url;
    cpr::Response response;
    constexpr int response_status_ok = 200;

    // 1.0. Configure target URL (NBU official open API)
    url = cpr::Url{"https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange?valcode=USD&json"};

    response = cpr::Get(url);  // Execute standard GET request
    if(response.status_code == response_status_ok)
        return AString(response.text.c_str(), static_cast<long long>(response.text.size()));

    return AString();
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Network::Send_Message(long long chat_id, long long message_thread_id, const char *text) const
{
    std::vector<cpr::Pair> payload_fields;
    std::string url_method_str;
    cpr::Url url;
    cpr::Response response;
    constexpr int response_status_ok = 200;
    
    // 1.0. Initialize url and build payload fields
    url_method_str = Pimpl->API_URL_SEND_MESSAGE;  // Get API url for sending msg`s
    url = cpr::Url { url_method_str };  // Build target url
    payload_fields =
    {
        {"chat_id", std::to_string(chat_id) },  // In field chat_id write chat_id
        {"text", text}  // Inf field text write message text
    };

    // 1.1. Append topic id if message targets a specific thread
    if (message_thread_id != 0)  // if topic id not 0, add him to payload
        payload_fields.push_back( {"message_thread_id", std::to_string(message_thread_id) } );

    const cpr::Payload payload = cpr::Payload(payload_fields.begin(), payload_fields.end() );

    Pimpl->Response_Session.SetUrl(url);
    Pimpl->Response_Session.SetPayload(payload);

    // 2.0. Execute API request, send POST request to Telegram API to send message and print logs
    response = Pimpl->Response_Session.Post();
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
    cpr::Url url;
    cpr::Response response;
    constexpr int response_status_ok = 200;

    // 1.0. Initialize url and build payload fields
    url_method_str = Pimpl->API_URL_SEND_MESSAGE;
    url = cpr::Url { url_method_str };
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
    // keyboard_json_str = R"({
    //     "inline_keyboard":
    //     [
    //         [{"text": "Accept", "callback_data": "action_accept"}],
    //         [{"text": "Decline", "callback_data": "action_decline"}],
    //         [{"text": "Help", "callback_data": "action_help"}],
    //         [{"text": "Exit", "callback_data": "action_exit"}]
    //     ]
    // })";
    // payload_fields.push_back({"reply_markup", std::move(keyboard_json_str) } );  // Attach keyboard to payload.
    keyboard_json_str = R"({
        "inline_keyboard": [
            [
                {"text": "Accept", "callback_data": "action_accept"},
                {"text": "Decline", "callback_data": "action_decline"},
                {"text": "Help", "callback_data": "action_help"},
                {"text": "Exit", "callback_data": "action_exit"}
            ]
        ]
    })";
    payload_fields.push_back({"reply_markup", std::move(keyboard_json_str) } );  // Attach keyboard to payload.

    const cpr::Payload payload = cpr::Payload(payload_fields.begin(), payload_fields.end() );
        // 1.1. Apply options to response session
    Pimpl->Response_Session.SetUrl(url);
    Pimpl->Response_Session.SetPayload(payload);

    // 2.0. Execute API request, send POST request to Telegram API to send message and print logs
    response = Pimpl->Response_Session.Post();
    if(response.status_code == response_status_ok)  // Output operation outcome details to logs
        std::println("Message reply sent successfully to chat: {}", chat_id);
    else
        std::println("Failed to send reply. Error: {}, Response: {}", response.status_code, response.text);
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Network::Answer_Callback_Query(const AString &callback_query_id) const
{
    std::string str_allert("Show allert example");
    cpr::Url url;
    cpr::Parameters url_param;
    cpr::Response response;
    constexpr int response_status_ok = 200;

    // 1.0. Setup target and fast response parameters
    url = cpr::Url { Pimpl->API_URL_ANSWER_CALLBACK_QUERY };
    const cpr::Payload payload = cpr::Payload
    {
        {"callback_query_id", callback_query_id.Get_C_Str() }
    };
    const cpr::Payload payload_test = cpr::Payload
    {
        {"callback_query_id", callback_query_id.Get_C_Str() },
        {"text", str_allert },
        {"show_alert", "true"} 
    };

    // 1.1. Apply options to response session
    Pimpl->Response_Session.SetUrl(url);
    Pimpl->Response_Session.SetPayload(payload);

    // 1.2. Execute instant request over pre-established connection
    response = Pimpl->Response_Session.Post();
    if(response.status_code == response_status_ok)  // Output operation outcome details to logs
        std::println("Answer call back sent successfully to chat: {}", response.status_code);
    else
        std::println("Failed to send reply. Error: {}, Response: {}", response.status_code, response.text);
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Network::Delete_Message(long long chat_id, long long message_id)
{
    cpr::Url url = cpr::Url(Pimpl->API_URL_DELETE_MESSAGE);
    const cpr::Payload payload = cpr::Payload
    {
        {"chat_id", std::to_string(chat_id)},
        {"message_id", std::to_string(message_id)}
    };

    Pimpl->Response_Session.SetUrl(url);
    Pimpl->Response_Session.SetPayload(payload);
    Pimpl->Response_Session.Post();

    std::println("Bot message was deleted");
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Network::Edit_Message_Text(long long chat_id, long long message_id, const AString &new_text_str)
{
    cpr::Url url = cpr::Url(Pimpl->API_URL_EDIT_MESSAGE_TEXT);
    const cpr::Payload payload = cpr::Payload
    {
        {"chat_id", std::to_string(chat_id)},
        {"message_id", std::to_string(message_id)},
        {"text", new_text_str.Get_C_Str()}
    };

    cpr::Post(url, payload);
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Network::Edit_Message_Reply_Markup(long long chat_id, long long message_id, const AString &markup_json_str)
{
    std::string keyboard_json_str;
    cpr::Url url = cpr::Url(Pimpl->API_URL_EDIT_MESSAGE_REPLY_MARKUP);

    // !!! TEMP EXAMPLE
    // 1.0. Define keyboard with web_app parameter, not work in group
    // keyboard_json_str = R"({
    //     "inline_keyboard": [
    //         [
    //             {"text": "Open Cabinet", "web_app": {"url": "https://animesss.com/" } }
    //         ]
    //     ]
    // })";
    // keyboard_json_str = R"({
    //     "inline_keyboard": [
    //         [
    //             {"text": "Open Cabinet", "url": "https://t.me/имя_вашего_бота/имя_приложения"}
    //         ]
    //     ]
    // })";
    keyboard_json_str = R"({
        "inline_keyboard": [
            [
                {"text": "Other", "callback_data": "action_accept"},
                {"text": "Difference", "callback_data": "action_decline"},
                {"text": "Nany", "callback_data": "action_help"},
                {"text": "I am done", "callback_data": "action_exit"}
            ]
        ]
    })";
    // !!! TEMP EXAMPLE END

    const cpr::Payload payload = cpr::Payload
    {
        {"chat_id", std::to_string(chat_id)},
        {"message_id", std::to_string(message_id)},
        {"reply_markup", keyboard_json_str}
    };

    cpr::Post(url, payload);
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Network::Set_My_Commands() const
{
    std::string commands_json_str;
    cpr::Url url;
    cpr::Response response;
    constexpr int response_status_ok = 200;

    url = cpr::Url{Pimpl->API_URL_SET_MY_COMMANDS};
    commands_json_str =
    R"([
        {"command": "enter", "description": "In progress"},
        {"command": "games", "description": "Show games"},
        {"command": "settings", "description": "In progress"}
    ])";
    const cpr::Payload payload = cpr::Payload
    {
        {"commands", commands_json_str}
    };

    response = cpr::Post(url, payload);
    if(response.status_code == response_status_ok)
        std::println("Menu commands successfully registered!");
    else
        std::println("Failed to register menu. Error: {}, Response: {}", response.status_code, response.text);
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Network::Delete_My_Commands() const
{
    cpr::Url url;
    cpr::Response response;
    constexpr int response_status_ok = 200;

    url = cpr::Url { Pimpl->API_URL_DELETE_MY_COMMANDS };

    response = cpr::Post(url);
    if(response.status_code == response_status_ok)
        std::println("Menu commands successfully deleted from Telegram servers.");
    else
        std::println("Failed to delete menu. Error: {}", response.status_code);
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Network::Send_Game_Web_App(long long chat_id, const char *text, const char *url_str) const
{
    std::string url_method_str;
    std::string keyboard_json_str;
    cpr::Url url;
    cpr::Response response;
    constexpr int response_status_ok = 200;

    // 1.0. Initialize method URL and target endpoint
    url_method_str = Pimpl->API_URL_SEND_MESSAGE;
    url = cpr::Url { url_method_str };

    // 1.1. Build inline keyboard payload with Phaser game link as Web App
    keyboard_json_str = R"({
        "inline_keyboard": [
            [
                {"text": "Play Phaser Game", "web_app": {"url": ")" + std::string(url_str) + R"("}}
            ]
        ]
    })";

    const cpr::Payload payload = cpr::Payload
    {
        {"chat_id", std::to_string(chat_id)},
        {"text", text},
        {"reply_markup", keyboard_json_str}
    };

    Pimpl->Response_Session.SetUrl(url);
    Pimpl->Response_Session.SetPayload(payload);

    // 1.2. Execute instant request over pre-established connection
    response = Pimpl->Response_Session.Post();
    if(response.status_code == response_status_ok)
        std::println("Web App message sent successfully to chat: {}", chat_id);
    else
        std::println("Failed to send Web App. Error: {}, Response: {}", response.status_code, response.text);
}
//------------------------------------------------------------------------------------------------------------
