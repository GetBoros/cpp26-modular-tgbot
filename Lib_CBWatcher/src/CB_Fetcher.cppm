//------------------------------------------------------------------------------------------------------------
module;

export module ACB_Fetcher;
//------------------------------------------------------------------------------------------------------------
import std;
//------------------------------------------------------------------------------------------------------------
export class ACB_Fetcher
{
public:
    ACB_Fetcher() = default;

    std::string Find_Access_Token(std::string_view room_name);
    std::string Get_Channel() const { return Last_Channel; }

private:
    std::string Extract_Field(const std::string &source, const std::string &key);
    std::string Request_Auth_Token(const std::string &room_name, const std::string &room_uid, const std::string &viewer_uid, const std::string &csrf_token, const std::string &session_id);

    std::string Last_Token;
    std::string Last_Channel;
};
//------------------------------------------------------------------------------------------------------------
