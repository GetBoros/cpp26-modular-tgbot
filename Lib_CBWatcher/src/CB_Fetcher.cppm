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

private:
    std::string Extract_Field(const std::string &source, const std::string &key);
    std::string Request_Auth_Token(const std::string &room_uid, const std::string &viewer_uid, const std::string &csrf_token);

    std::string Last_Token;
    std::string Last_Channel;
};
//------------------------------------------------------------------------------------------------------------
