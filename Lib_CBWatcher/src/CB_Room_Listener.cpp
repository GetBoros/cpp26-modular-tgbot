//------------------------------------------------------------------------------------------------------------
module;
#include <ixwebsocket/IXWebSocket.h>
module ACB_Room_Listener;
//------------------------------------------------------------------------------------------------------------
import std;
//------------------------------------------------------------------------------------------------------------





// ACB_Room_Listener
struct ACB_Room_Listener::SImpl
{
    bool Is_Running;

    std::string Channel_Name;
    ix::WebSocket Web_Socket;
};
//------------------------------------------------------------------------------------------------------------
std::string Get_Field(const std::string &source_str, const std::string &target_key)
{
    size_t start_pos;
    size_t key_pos;
    size_t end_pos;
    std::string full_key;
    std::string result_str;

    // 1.0. Locate data payload offset to avoid matching header structures
    start_pos = source_str.find("\"data\"");
    if(start_pos == std::string::npos)
        start_pos = 0;

    // 1.1. Search for key surrounded by quotes
    full_key = "\"" + target_key + "\"";
    key_pos = source_str.find(full_key, start_pos);
    if(key_pos == std::string::npos)
    {
        key_pos = source_str.find(target_key, start_pos);
        if(key_pos == std::string::npos)
            return "";
    }

    // 1.2. Advance past the colon delimiter
    key_pos = source_str.find(':', key_pos);
    if(key_pos == std::string::npos)
        return "";
    key_pos++;

    // 1.3. Skip whitespace, quotes and backslashes
    while( (key_pos < source_str.size() ) && ( (source_str[key_pos] == ' ') || (source_str[key_pos] == '\\') || (source_str[key_pos] == '\"') ) )
        key_pos++;

    // 1.4. Find terminal delimiter
    end_pos = key_pos;
    while(end_pos < source_str.size() )
    {
        if( (source_str[end_pos] == '\\') && (end_pos + 1 < source_str.size() ) && (source_str[end_pos + 1] == '\"') )
        {
            end_pos += 2;

            break;
        }
        if( (source_str[end_pos] == '\"') || (source_str[end_pos] == ',') )
        {
            break;
        }
        end_pos++;
    }

    result_str = source_str.substr(key_pos, end_pos - key_pos);

    return result_str;
}
//------------------------------------------------------------------------------------------------------------
ACB_Room_Listener::ACB_Room_Listener()
{
    Pimpl = new SImpl();
    Pimpl->Is_Running = false;
}
//------------------------------------------------------------------------------------------------------------
ACB_Room_Listener::~ACB_Room_Listener()
{
    Stop();

    if(Pimpl != 0)
    {
        delete Pimpl;

        Pimpl = 0;
    }
}
//------------------------------------------------------------------------------------------------------------
void ACB_Room_Listener::Start_Listening(std::string_view access_token, std::string_view channel_name)
{
    std::string url_str;
    ix::WebSocketHttpHeaders custom_headers;

    // 1.0. Stop previous session if active
    Stop();

    // 1.1. Cache active channel name
    Pimpl->Channel_Name = std::string(channel_name);

    // 2.0. Construct realtime WebSocket endpoint URL
    url_str = std::format("wss://realtime.pa.highwebmedia.com/?access_token={}&format=json&heartbeats=true&v=3", access_token);
    Pimpl->Web_Socket.setUrl(url_str);

    // 2.1. Prepare browser headers
    custom_headers["User-Agent"] = "Mozilla/5.0 (X11; Linux x86_64; rv:128.0) Gecko/20100101 Firefox/128.0";
    custom_headers["Origin"] = "https://chaturbate.com";
    Pimpl->Web_Socket.setExtraHeaders(custom_headers);

    // 3.0. Configure WebSocket message handler
    Pimpl->Web_Socket.setOnMessageCallback([this](const ix::WebSocketMessagePtr &msg)
    {
        int amount_num;
        std::string amount_str;
        std::string sender_name;
        std::string message_text;
        std::string subscribe_payload;

        if(msg->type == ix::WebSocketMessageType::Open)
            std::println("\033[32m[+] Realtime connection established successfully!\033[0m");
        else if(msg->type == ix::WebSocketMessageType::Close)
            std::println("\033[31m[-] Realtime connection closed: {}\033[0m", msg->closeInfo.reason);
        else if(msg->type == ix::WebSocketMessageType::Error)
            std::println("\033[31m[-] Realtime network error: {}\033[0m", msg->errorInfo.reason);
        else if(msg->type == ix::WebSocketMessageType::Message)
        {
            // 3.1. Filter heartbeat signals
            if(msg->str.find("{\"action\":0}") != std::string::npos)
                return;

            // 3.2. Handle server authorization errors
            if(msg->str.find("\"action\":9") != std::string::npos)
            {
                std::println("\033[31m[!] Server error response: {}\033[0m", msg->str);

                return;
            }

            // 3.3. Execute room channel subscription upon successful connection
            if(msg->str.find("\"action\":4") != std::string::npos)
            {
                std::println("\033[32m[+] Authorized! Subscribing to channel: {}...\033[0m", Pimpl->Channel_Name);
                subscribe_payload = std::format(R"({{"action":10,"channel":"{}","params":{{}},"flags":327680}})", Pimpl->Channel_Name);
                Pimpl->Web_Socket.send(subscribe_payload);

                return;
            }

            // 3.4. Confirm active subscription
            if(msg->str.find("\"action\":11") != std::string::npos)
            {
                std::println("\033[36m[+] Channel subscription confirmed (ATTACHED)!\033[0m\n");

                return;
            }

            // 3.5. Parse tip alert events
            if(msg->str.find("RoomTipAlertTopic") != std::string::npos)
            {
                amount_str = Get_Field(msg->str, "amount");
                amount_num = amount_str.empty() ? 0 : std::stoi(amount_str);

                sender_name = Get_Field(msg->str, "from_username");
                if(sender_name.empty() == true)
                    sender_name = "Anonymous";

                std::println("\033[1;32m[💰 TIP]: {} sent {} tokens!\033[0m", sender_name, amount_num);

                if(amount_num >= 500)
                    std::println("\n\033[1;41;37m >>> ALERT: LARGE TIP ({} tokens)! CHECK STREAM! <<< \033[0m\a\n", amount_num);
            }
            else if(msg->str.find("RoomMessageTopic") != std::string::npos)  // Parse regular chat messages
            {
                sender_name = Get_Field(msg->str, "username");
                message_text = Get_Field(msg->str, "message");

                if( (sender_name.empty() == false) && (message_text.empty() == false) )
                    std::println("\033[36m[CHAT] {}: \033[0m{}", sender_name, message_text);
            }
        }
    });

    // 4.0. Launch background network thread
    Pimpl->Web_Socket.start();
    Pimpl->Is_Running = true;
}
//------------------------------------------------------------------------------------------------------------
void ACB_Room_Listener::Stop()
{
    if(Pimpl == 0)
        return;

    if(Pimpl->Is_Running == true)
    {
        Pimpl->Web_Socket.stop();
        Pimpl->Is_Running = false;
        
        std::println("[-] Room listener background worker stopped.");
    }
}
//------------------------------------------------------------------------------------------------------------