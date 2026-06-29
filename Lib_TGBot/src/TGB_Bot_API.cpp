//------------------------------------------------------------------------------------------------------------
module;
#include <print>
#include <cpr/cpr.h>
module TGB_Bot_API;
//------------------------------------------------------------------------------------------------------------
import TGB_Deserializer;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
struct SPimpl
{
    void Initialize()
    {
        const char *env_token;
        std::string bot_token;

        // 1.0. Retrieve credentials from environment variables
        env_token = std::getenv("TELEGRAM_BOT_TOKEN");
        if(env_token != 0)
            bot_token = env_token;

        // 1.1. Construct Telegram Bot API endpoints
        API_URL_GET_UPDATES = "https://api.telegram.org/bot" + bot_token + "/getUpdates";
        API_URL_SEND_MESSAGE = "https://api.telegram.org/bot" + bot_token + "/sendMessage";
        API_URL_EDIT_MESSAGE_TEXT = "https://api.telegram.org/bot" + bot_token + "/editMessageText";
        API_URL_EDIT_MESSAGE_REPLY_MARKUP = "https://api.telegram.org/bot" + bot_token + "/editMessageReplyMarkup";
        API_URL_DELETE_MESSAGE = "https://api.telegram.org/bot" + bot_token + "/deleteMessage";
        API_URL_ANSWER_CALLBACK_QUERY = "https://api.telegram.org/bot" + bot_token + "/answerCallbackQuery";
        API_URL_SET_MY_COMMANDS = "https://api.telegram.org/bot" + bot_token + "/setMyCommands";
        API_URL_DELETE_MY_COMMANDS = "https://api.telegram.org/bot" + bot_token + "/deleteMyCommands";
    }

    void Execute_Post_Request(cpr::Url url, const cpr::Payload &payload = cpr::Payload { } )
    {
        constexpr int response_status_ok = 200;

        cpr::Response response;

        Response_Session.SetUrl(url);
        Response_Session.SetPayload(payload);

        // 1.2. Execute instant request over pre-established connection
        response = Response_Session.Post();
        if(response.status_code == response_status_ok)
            std::println("Execute Post message sent successfully to chat: {}", response_status_ok);
        else
            std::println("Execute Post message send Failed Error: {}, Response: {}", response.status_code, response.text);
    }

    int Lst_Processed_Event_Id;

    std::string API_URL_GET_UPDATES;
    std::string API_URL_SEND_MESSAGE;
    std::string API_URL_EDIT_MESSAGE_TEXT;
    std::string API_URL_EDIT_MESSAGE_REPLY_MARKUP;
    std::string API_URL_DELETE_MESSAGE;
    std::string API_URL_ANSWER_CALLBACK_QUERY;
    std::string API_URL_SET_MY_COMMANDS;
    std::string API_URL_DELETE_MY_COMMANDS;

    std::string Last_Response;
    
    cpr::Session Polling_Session;
    cpr::Session Response_Session;
    
    ATGB_Deserializer Deserializer;

};
//------------------------------------------------------------------------------------------------------------




// ATGB_Bot_API
ATGB_Bot_API::~ATGB_Bot_API()
{
    Delete_My_Commands();

    delete Pimpl;
}
//------------------------------------------------------------------------------------------------------------
ATGB_Bot_API::ATGB_Bot_API()
{
    Pimpl = new SPimpl();

    Pimpl->Initialize();
    Pimpl->Polling_Session.SetUrl(cpr::Url{Pimpl->API_URL_GET_UPDATES} );
    
    Initialize();
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Bot_API::Initialize()
{
    Set_My_Commands();  // Setup UI elements and menus on Telegram servers

    std::println("Bot started! Waiting for messages...");
}
//------------------------------------------------------------------------------------------------------------
STelegram_Event ATGB_Bot_API::Poll_Events_Temp()
{
    int update_id;
    AString response_text;
    cpr::Parameters parameters;
    cpr::Response response;
    STelegram_Event telegram_event;
    constexpr int response_status_ok = 200;

    // 1.0. Configure request targets and parameters
    update_id = Pimpl->Lst_Processed_Event_Id;
    parameters = cpr::Parameters { {"timeout", "10"}, {"offset", std::to_string(update_id + 1) } };
    Pimpl->Polling_Session.SetParameters(parameters); // Update session settings

    // 1.1. Execute persistent request and update buffer in-place
    response = Pimpl->Polling_Session.Get(); // Wait 10 seconds.
    if(response.status_code == response_status_ok)
    {
        response_text = AString(response.text.c_str(), static_cast<long long>(response.text.size() ) );

        Pimpl->Deserializer.Deserialize_Event(response_text, telegram_event);

        // 1.2. Safeguard transaction ID to prevent zero-index reset loops
        if(telegram_event.Update_Id > Pimpl->Lst_Processed_Event_Id)
            Pimpl->Lst_Processed_Event_Id = telegram_event.Update_Id;

        return telegram_event;
    }
    else
    {
        std::println("Network Error: {}", response.status_code);
        std::this_thread::sleep_for(std::chrono::seconds(1) );
    }

    return telegram_event;
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Bot_API::Poll_Events(int update_id, AString &response_result) const
{
    cpr::Parameters parameters;
    cpr::Response response;
    constexpr int response_status_ok = 200;

    // 1.0. Configure request targets and parameters
    parameters = cpr::Parameters { {"timeout", "10"}, {"offset", std::to_string(update_id + 1) } };
    Pimpl->Polling_Session.SetParameters(parameters);  // Update session settings

    // 1.1. Execute persistent request and update buffer in-place
    response = Pimpl->Polling_Session.Get();  // wait 10 second if not have new msgs
    if(response.status_code == response_status_ok)
        response_result.Assign(response.text.c_str(), static_cast<long long>(response.text.size() ) );
    else
    {
        response_result.Clear();

        std::println("Network Error: {}", response.status_code);
        std::this_thread::sleep_for(std::chrono::seconds(1) );
    }
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Bot_API::Send_Message(long long chat_id, long long message_thread_id, const char *text) const
{
    cpr::Url url;
    std::vector<cpr::Pair> payload_fields;
    
    // 1.0. Initialize url and build payload fields
    url = cpr::Url{Pimpl->API_URL_SEND_MESSAGE};  // Build target url
    payload_fields =
    {
        {"chat_id", std::to_string(chat_id) },  // In field chat_id write chat_id
        {"text", text}  // Inf field text write message text
    };

    // 1.1. Append topic id if message targets a specific thread
    if (message_thread_id != 0)  // if topic id not 0, add him to payload
        payload_fields.push_back( {"message_thread_id", std::to_string(message_thread_id) } );

    const cpr::Payload payload = cpr::Payload(payload_fields.begin(), payload_fields.end() );

    Pimpl->Execute_Post_Request(url, payload);
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Bot_API::Send_Message_Reply(long long chat_id, long long message_thread_id, long long message_id, const char *text) const
{
    cpr::Url url;
    std::vector<cpr::Pair> payload_fields;
    std::string reply_json_str;
    std::string keyboard_json_str;

    // 1.0. Initialize url and build payload fields
    url = cpr::Url{Pimpl->API_URL_SEND_MESSAGE};
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

    Pimpl->Execute_Post_Request(url, payload);
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Bot_API::Answer_Callback_Query(const AString &callback_query_id) const
{
    int cache_time_sec = 3600;
    std::string str_allert("Show allert example");
    std::string deep_link_url("t.me/get_boros_cpp_bot?start=test_id_36");
    cpr::Url url;
    cpr::Parameters url_param;

    // 1.0. Setup target and fast response parameters
    url = cpr::Url{ Pimpl->API_URL_ANSWER_CALLBACK_QUERY };
    const cpr::Payload payload = cpr::Payload
    {
        {"callback_query_id", callback_query_id.Get_C_Str() }
    };
    const cpr::Payload payload_test = cpr::Payload
    {
        {"callback_query_id", callback_query_id.Get_C_Str() },
        {"text", str_allert },
        {"cache_time", std::to_string(cache_time_sec) },
        {"show_alert", "false"} 
    };
    const cpr::Payload payload_link = cpr::Payload
    {
        {"callback_query_id", callback_query_id.Get_C_Str() },
        {"url", deep_link_url.c_str() }
    };

    // 1.1. Apply options to response session
    Pimpl->Execute_Post_Request(url, payload);
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Bot_API::Delete_Message(long long chat_id, long long message_id)
{
    cpr::Url url = cpr::Url(Pimpl->API_URL_DELETE_MESSAGE);
    const cpr::Payload payload = cpr::Payload
    {
        {"chat_id", std::to_string(chat_id) },
        {"message_id", std::to_string(message_id) }
    };

    Pimpl->Execute_Post_Request(url, payload);
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Bot_API::Edit_Message_Text(long long chat_id, long long message_id, const AString &new_text_str)
{
    cpr::Url url = cpr::Url(Pimpl->API_URL_EDIT_MESSAGE_TEXT);
    const cpr::Payload payload = cpr::Payload
     {
        {"chat_id", std::to_string(chat_id) },
        {"message_id", std::to_string(message_id) },
        {"text", new_text_str.Get_C_Str() }
    };

    Pimpl->Execute_Post_Request(url, payload);
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Bot_API::Edit_Message_Reply_Markup(long long chat_id, long long message_id, const AString &markup_json_str)
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

    Pimpl->Execute_Post_Request(url, payload);
}
//------------------------------------------------------------------------------------------------------------
AString ATGB_Bot_API::Get_NBU_USD_Rate() const
{
    cpr::Url url;
    cpr::Response response;
    constexpr int response_status_ok = 200;

    // 1.0. Configure target URL (NBU official open API)
    url = cpr::Url{"https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange?valcode=USD&json"};

    response = cpr::Get(url);  // Execute standard GET request
    if(response.status_code == response_status_ok)
        return AString(response.text.c_str(), static_cast<long long>(response.text.size() ) );

    return AString();
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Bot_API::Set_My_Commands() const
{
    std::string commands_json_str;
    cpr::Url url;

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

    Pimpl->Execute_Post_Request(url, payload);
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Bot_API::Delete_My_Commands() const
{
    cpr::Url url = cpr::Url{ Pimpl->API_URL_DELETE_MY_COMMANDS };

    Pimpl->Execute_Post_Request(url);
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Bot_API::Send_Game_Web_App(long long chat_id, const char *text, const char *url_str) const
{
    std::string url_method_str;
    std::string keyboard_json_str;
    cpr::Url url;

    // 1.0. Initialize method URL and target endpoint
    url_method_str = Pimpl->API_URL_SEND_MESSAGE;
    url = cpr::Url{ url_method_str };

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

    Pimpl->Execute_Post_Request(url, payload);
}
//------------------------------------------------------------------------------------------------------------
