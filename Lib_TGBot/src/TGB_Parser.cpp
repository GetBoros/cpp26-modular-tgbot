//------------------------------------------------------------------------------------------------------------
module;
#include <meta>
#include <print>
#include <nlohmann/json.hpp>
module TGB_Parser;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
struct SPimpl
{
    std::string Response_Text;

};
//------------------------------------------------------------------------------------------------------------
class AChat
{
public:
    long long Id;

};
//------------------------------------------------------------------------------------------------------------
class AUser_Info
{
public:
    long long Id;
    bool Is_Bot;

    std::string First_Name;
};
//------------------------------------------------------------------------------------------------------------
class AMessage
{
public:
    long long Message_Thread_Id;

    std::string Text;
    
    AUser_Info From;
    AChat Chat;
};
//------------------------------------------------------------------------------------------------------------
template <size_t size> consteval std::array<char, size> To_Lower_Case(std::string_view input)
{
    std::array<char, size> result {};

    for(size_t i = 0; i < input.size(); i++)
    {
        bool is_upper = (input[i] >= 'A') && (input[i] <= 'Z');

        if(is_upper == true)
            result[i] = input[i] + ('a' - 'A');
        else
            result[i] = input[i];
    }
    return result;
}
//------------------------------------------------------------------------------------------------------------
template <typename type_name> void Parse_Json(type_name &object, const nlohmann::json &json_data);
//------------------------------------------------------------------------------------------------------------
template <typename field_type> void Assign_Field(field_type &field, const nlohmann::json &json_node)
{
    constexpr bool is_class = std::is_class_v<field_type>;
    constexpr bool is_string = std::is_same_v<field_type, std::string>;

    if constexpr( (is_class == true) && (is_string == false) )
    {
        Parse_Json(field, json_node);
    }
    else
    {
        field = json_node.get<field_type>();
    }
}
//------------------------------------------------------------------------------------------------------------
template <typename type_name> void Parse_Json(type_name &object, const nlohmann::json &json_data)
{
    constexpr std::meta::info meta_info = ^^type_name;
    constexpr auto access_ctx = std::meta::access_context::unchecked();
    static constexpr auto fields = std::define_static_array(std::meta::nonstatic_data_members_of(meta_info, access_ctx) );
    static constexpr auto members = std::define_static_array(std::meta::members_of(meta_info, access_ctx) );

    template for(constexpr auto field : fields)
    {
        constexpr auto field_name = std::meta::identifier_of(field);
        constexpr auto json_key_array = To_Lower_Case<field_name.size() + 1>(field_name);
        const std::string_view json_key(json_key_array.data(), field_name.size() );

        bool has_key = json_data.contains(json_key);
        if(has_key == true)
            Assign_Field(object.[:field:], json_data[json_key]);
    }
}
//------------------------------------------------------------------------------------------------------------




// ATGB_Parser
ATGB_Parser::ATGB_Parser()
{
    Pimpl = new SPimpl();
}
//------------------------------------------------------------------------------------------------------------
int ATGB_Parser::Set_Response_Text(const char *response_text, long long &id_chat, long long &id_chat_topic)
{
    int last_update_id;

    last_update_id = 0;

    try
    {
        nlohmann::json json_data = nlohmann::json::parse(response_text);

        for (auto const &item : json_data["result"])
        {
            last_update_id = item["update_id"];  // set new last update id, if need get next msg
            
            std::string pretty_json = json_data.dump(4);
            std::println("Received JSON:\n{}", pretty_json);

            if (item.contains("message") && item["message"].contains("text") )
            {
                AMessage message;

                Parse_Json(message, item["message"]);

                id_chat = message.Chat.Id;
                id_chat_topic = message.Message_Thread_Id;
                std::string message_text = message.Text;
                std::string user_name = message.From.First_Name;

                std::println("Message text: {}, Thread ID {}", message.Text, message.Message_Thread_Id);
            }
        }
    }
    catch (nlohmann::json::exception const &e)
    {
        std::println("JSON Error: {}", e.what() );
    }

    return last_update_id;
}
//------------------------------------------------------------------------------------------------------------
/*
std::string pretty_json = json_data.dump(4);
std::println("Received JSON:\n{}", pretty_json);
Received JSON:
{
    "ok": true,
    "result": [
        {
            "message": {
                "chat": {
                    "id": -1003763586627,
                    "is_forum": true,
                    "title": "Diary",
                    "type": "supergroup"
                },
                "date": 1781806303,
                "from": {
                    "first_name": "Andrey",
                    "id": 1775156303,
                    "is_bot": false,
                    "language_code": "en",
                    "username": "GetBoros"
                },
                "is_topic_message": true,
                "message_id": 57,
                "message_thread_id": 17,
                "reply_to_message": {
                    "chat": {
                        "id": -1003763586627,
                        "is_forum": true,
                        "title": "Diary",
                        "type": "supergroup"
                    },
                    "date": 1774943035,
                    "forum_topic_created": {
                        "icon_color": 16749490,
                        "name": "TEMP"
                    },
                    "from": {
                        "first_name": "Andrey",
                        "id": 1775156303,
                        "is_bot": false,
                        "language_code": "en",
                        "username": "GetBoros"
                    },
                    "is_topic_message": true,
                    "message_id": 17,
                    "message_thread_id": 17
                },
                "text": "Hello"
            },
            "update_id": 875409081
        }
    ]
}
*/
//------------------------------------------------------------------------------------------------------------
