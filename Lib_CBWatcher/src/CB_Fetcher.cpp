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

    // 1.1. Advance past the colon delimiter
    key_pos = source.find(':', key_pos);
    if(key_pos == std::string::npos)
    {
        return "";
    }
    key_pos++;

    // 1.2. Skip initial spaces
    while( (key_pos < source.size()) && (source[key_pos] == ' ') )
    {
        key_pos++;
    }

    // 1.3. Locate terminal delimiter (comma or closing brace)
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
//------------------------------------------------------------------------------------------------------------
std::string ACB_Fetcher::Request_Auth_Token(const std::string &room_uid, const std::string &viewer_uid, const std::string &csrf_token)
{
    std::string presence_id;
    std::string topics_json;
    std::string token_str;
    cpr::Response auth_res;

    // 1.0. Prepare presence identifier and topic subscription payload
    presence_id = "+anon12345678";
    topics_json = "{\"RoomUserPresenceTopic#RoomUserPresenceTopic:" + room_uid + ":" + viewer_uid + "\":{\"broadcaster_uid\":\"" + room_uid + "\",\"user_uid\":\"" + viewer_uid + "\"},\"GlobalPushServiceBackendChangeTopic#GlobalPushServiceBackendChangeTopic\":{},\"RoomTipAlertTopic#RoomTipAlertTopic:" + room_uid + "\":{\"broadcaster_uid\":\"" + room_uid + "\"},\"RoomMessageTopic#RoomMessageTopic:" + room_uid + "\":{\"broadcaster_uid\":\"" + room_uid + "\"}}";

    // 2.0. Execute POST request to obtain WebSocket authorization
    auth_res = cpr::Post(
        cpr::Url{"https://chaturbate.com/push_service/auth/"},
        cpr::Header{
            {"User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:128.0) Gecko/20100101 Firefox/128.0"},
            {"X-CSRFToken", csrf_token},
            {"X-Requested-With", "XMLHttpRequest"},
            {"Origin", "https://chaturbate.com"}
        },
        cpr::Cookies{
            {"csrftoken", csrf_token},
            {"agreeterms", "1"}
        },
        cpr::Multipart{
            {"backend", "a"},
            {"csrfmiddlewaretoken", csrf_token},
            {"presence_id", presence_id},
            {"topics", topics_json}
        }
    );

    if(auth_res.status_code != 200)
    {
        std::println("[-] Error: auth endpoint returned status code {}", auth_res.status_code);
        return "";
    }

    // 3.0. Extract access token and channel identifier from response JSON
    token_str = Extract_Field(auth_res.text, "token");
    if(token_str == "null")
    {
        std::println("[-] Auth endpoint rejected topics. Raw response: {}", auth_res.text);
        return "";
    }

    Last_Channel = Extract_Field(auth_res.text, "RoomTipAlertTopic#RoomTipAlertTopic:" + room_uid);

    return token_str;
}
//------------------------------------------------------------------------------------------------------------
std::string ACB_Fetcher::Find_Access_Token(std::string_view room_name)
{
    bool is_public;
    std::string target_url;
    std::string csrf_token;
    std::string room_status;
    std::string room_uid;
    std::string viewer_uid;
    cpr::Response page_res;

    // 1.0. Request main room webpage
    std::println("[1/3] Fetching webpage for room: {}", room_name);
    target_url = "https://chaturbate.com/" + std::string(room_name) + "/";

    page_res = cpr::Get(
        cpr::Url{target_url},
        cpr::Header{
            {"User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:128.0) Gecko/20100101 Firefox/128.0"}
        },
        cpr::Cookies{
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

    // 2.1. Fallback to extracting from HTML if cookie container was empty
    if(csrf_token.empty() == true)
    {
        csrf_token = Extract_Field(page_res.text, "csrftoken");
    }

    // 2.2. Verify room status and retrieve room identifiers
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
    Last_Token = Request_Auth_Token(room_uid, viewer_uid, csrf_token);

    if(Last_Token.empty() == false)
    {
        std::println("[3/3] Token successfully acquired! Token length: {}", Last_Token.size());
    }

    return Last_Token;
}
//------------------------------------------------------------------------------------------------------------