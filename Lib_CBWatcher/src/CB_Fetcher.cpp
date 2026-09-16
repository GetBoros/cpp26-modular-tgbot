//------------------------------------------------------------------------------------------------------------
module;
#include <cpr/cpr.h>
module ACB_Fetcher;
//------------------------------------------------------------------------------------------------------------
import std;
//------------------------------------------------------------------------------------------------------------




// ACB_Fetcher
std::string ACB_Fetcher::Extract_Field(const std::string &source, const std::string &key)
{
    size_t key_pos;
    size_t val_start;
    size_t val_end;
    std::string result_str;

    // 1.0. Search for field key in source text
    key_pos = source.find(key);
    if(key_pos == std::string::npos)
    {
        return "";
    }

    // 1.1. Advance past the key and find delimiter colon
    key_pos = source.find(':', key_pos + key.size());
    if(key_pos == std::string::npos)
    {
        return "";
    }
    key_pos++;

    // 1.2. Skip whitespace
    while( (key_pos < source.size()) && (source[key_pos] == ' ') )
    {
        key_pos++;
    }

    // 1.3. Locate terminal delimiter
    val_start = key_pos;
    val_end = source.find_first_of(",}", val_start);
    if(val_end == std::string::npos)
    {
        val_end = source.size();
    }

    result_str = source.substr(val_start, val_end - val_start);

    // 1.4. Trim leading quotes, unicode escapes (\u0022), backslashes and spaces
    while(result_str.empty() == false)
    {
        if(result_str.starts_with("\\u0022") == true)
        {
            result_str.erase(0, 6);
        }
        else if(result_str.starts_with("u0022") == true)
        {
            result_str.erase(0, 5);
        }
        else if( (result_str.front() == '\"') || (result_str.front() == '\\') || (result_str.front() == ' ') )
        {
            result_str.erase(0, 1);
        }
        else
        {
            break;
        }
    }

    // 1.5. Trim trailing quotes, unicode escapes (\u0022), backslashes and spaces
    while(result_str.empty() == false)
    {
        if(result_str.ends_with("\\u0022") == true)
        {
            result_str.erase(result_str.size() - 6);
        }
        else if(result_str.ends_with("u0022") == true)
        {
            result_str.erase(result_str.size() - 5);
        }
        else if( (result_str.back() == '\"') || (result_str.back() == '\\') || (result_str.back() == ' ') )
        {
            result_str.pop_back();
        }
        else
        {
            break;
        }
    }

    return result_str;
}

std::string ACB_Fetcher::Request_Auth_Token(const std::string &room_name, const std::string &room_uid, const std::string &viewer_uid, const std::string &csrf_token, const std::string &session_id)
{
    size_t ch_pos;
    size_t ch_end;
    std::string presence_id;
    std::string topics_json;
    std::string token_str;
    cpr::Response auth_res;

    // 1.0. Prepare 11-character presence identifier
    presence_id = "+q1w2e3r4t5y";

    // 1.1. Construct presence and subscription topics payload for authenticated viewer
    topics_json = "{"
        "\"GlobalPushServiceBackendChangeTopic#GlobalPushServiceBackendChangeTopic\":{},"
        "\"RoomUserPresenceTopic#RoomUserPresenceTopic:" + room_uid + ":" + viewer_uid + "\":{\"broadcaster_uid\":\"" + room_uid + "\",\"user_uid\":\"" + viewer_uid + "\"},"
        "\"RoomStatusTopic#RoomStatusTopic:" + room_uid + "\":{\"broadcaster_uid\":\"" + room_uid + "\"},"
        "\"RoomNoticeTopic#RoomNoticeTopic:" + room_uid + "\":{\"broadcaster_uid\":\"" + room_uid + "\"},"
        "\"RoomUpdateTopic#RoomUpdateTopic:" + room_uid + "\":{\"broadcaster_uid\":\"" + room_uid + "\"},"
        "\"RoomMessageTopic#RoomMessageTopic:" + room_uid + "\":{\"broadcaster_uid\":\"" + room_uid + "\"},"
        "\"RoomTipAlertTopic#RoomTipAlertTopic:" + room_uid + "\":{\"broadcaster_uid\":\"" + room_uid + "\"}"
    "}";

    // 2.0. Execute POST request to obtain WebSocket authorization using active session
    auth_res = cpr::Post(
        cpr::Url{"https://chaturbate.com/push_service/auth/"},
        cpr::Header{
            {"User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:128.0) Gecko/20100101 Firefox/128.0"},
            {"X-CSRFToken", csrf_token},
            {"X-Requested-With", "XMLHttpRequest"},
            {"Origin", "https://chaturbate.com"},
            {"Referer", "https://chaturbate.com/" + room_name + "/"}
        },
        cpr::Cookies{
            {"csrftoken", csrf_token},
            {"sessionid", session_id},
            {"agreeterms", "1"}
        },
        cpr::Multipart{
            {"backend", "a"},
            {"csrfmiddlewaretoken", csrf_token},
            {"presence_id", presence_id},
            {"topics", topics_json}
        }
    );

    // 2.1. Validate HTTP response status
    if(auth_res.status_code != 200)
    {
        std::println("[-] Error: auth endpoint returned status code {}", auth_res.status_code);
        return "";
    }

    // 2.2. Debug print raw server response
    std::println("[DEBUG] Server Response: {}", auth_res.text);

    // 3.0. Extract access token from response JSON
    token_str = Extract_Field(auth_res.text, "token");
    if(token_str == "null")
    {
        std::println("[-] Auth endpoint rejected topics. Raw response: {}", auth_res.text);
        return "";
    }

    // 4.0. Extract clean room channel identifier
    ch_pos = auth_res.text.find("room:grouped:");
    if(ch_pos != std::string::npos)
    {
        ch_end = auth_res.text.find('\"', ch_pos);
        if(ch_end != std::string::npos)
        {
            Last_Channel = auth_res.text.substr(ch_pos, ch_end - ch_pos);
        }
    }

    return token_str;
}

std::string ACB_Fetcher::Find_Access_Token(std::string_view room_name)
{
    bool is_public;
    const char *env_session;
    std::string session_id;
    std::string target_url;
    std::string csrf_token;
    std::string room_status;
    std::string room_uid;
    std::string viewer_uid;
    cpr::Response page_res;

    // 1.0. Retrieve session ID from environment or use fallback
    env_session = std::getenv("CB_SESSION_ID");
    if(env_session != 0)
    {
        session_id = env_session;
    }
    else
    {
        session_id = "w4tuvb0wccpyyc3l9ekcnvzx1bsb1pue"; // Default session cookie
    }

    // 1.1. Request main room webpage with session cookies
    std::println("[1/3] Fetching webpage for room: {}", room_name);
    target_url = "https://chaturbate.com/" + std::string(room_name) + "/";

    page_res = cpr::Get(
        cpr::Url{target_url},
        cpr::Header{
            {"User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:128.0) Gecko/20100101 Firefox/128.0"}
        },
        cpr::Cookies{
            {"sessionid", session_id},
            {"agreeterms", "1"}
        }
    );

    if(page_res.status_code != 200)
    {
        std::println("[-] Failed to fetch room page. Status: {}", page_res.status_code);
        return "";
    }

    // 2.0. Extract CSRF token from response cookies
    for(const cpr::Cookie &cookie : page_res.cookies)
    {
        if(cookie.GetName() == "csrftoken")
        {
            csrf_token = cookie.GetValue();
            break;
        }
    }

    if(csrf_token.empty() == true)
    {
        csrf_token = Extract_Field(page_res.text, "csrftoken");
    }

    // 2.1. Verify room status and retrieve identifiers
    room_status = Extract_Field(page_res.text, "room_status");
    is_public = (room_status == "public");

    if(is_public == false)
    {
        std::println("[-] Room is currently offline. Status: {}", room_status);
        return "";
    }

    room_uid = Extract_Field(page_res.text, "room_uid");
    viewer_uid = Extract_Field(page_res.text, "viewer_uid");

    std::println("[+] Room is ONLINE! UID: {}, Viewer: {}, CSRF: {}", room_uid, viewer_uid, csrf_token);

    // 3.0. Request WebSocket credentials
    std::println("[2/3] Requesting push service token...");
    Last_Token = Request_Auth_Token(std::string(room_name), room_uid, viewer_uid, csrf_token, session_id);

    if(Last_Token.empty() == false)
    {
        std::println("[3/3] Token successfully acquired! Token length: {}", Last_Token.size());
    }

    return Last_Token;
}
//------------------------------------------------------------------------------------------------------------
std::string ACB_Fetcher::Get_Channel() const
{
    return Last_Channel;
}
//------------------------------------------------------------------------------------------------------------
