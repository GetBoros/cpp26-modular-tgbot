//------------------------------------------------------------------------------------------------------------
module;
#include <nlohmann/json.hpp>

#include <meta>
#include <print>
module TGB_Parser;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
template <size_t size> consteval std::array<char, size> To_Lower_Case(std::string_view input)
{
    bool is_upper = false;
    std::array<char, size> result {};

    for(size_t i = 0; i < input.size(); i++)
    {
        is_upper = (input[i] >= 'A') && (input[i] <= 'Z');
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
    constexpr bool is_astring = std::is_same_v<field_type, AString>;
    constexpr bool is_std_string = std::is_same_v<field_type, std::string>;

    if constexpr(is_astring == true)
    {
        std::string str_temp = json_node.get<std::string>();

        field = AString(str_temp.c_str(), str_temp.size() );  // Convert nlohmann's std::string to our fast AString
    }
    else if constexpr(is_std_string == true)  // Just in case we still use std::string somewhere
        field = json_node.get<std::string>();
    else if constexpr(is_class == true)  // Recursion for nested structs (SUser_Info, SChat, etc.)
        Parse_Json(field, json_node);
    else  // Basic types (long long, bool, int)
        field = json_node.get<field_type>();
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
        constexpr auto field_name = std::meta::identifier_of(field);  // e.g., "From"
        constexpr auto json_key_array = To_Lower_Case<field_name.size() + 1>(field_name);  // "From" -> "from\0"
        const std::string_view json_key(json_key_array.data(), field_name.size() );  // Excludes null terminator

        if(json_data.contains(json_key) == true)
            Assign_Field(object.[:field:], json_data[json_key]);  // Pass to assignment helper
    }
}
//------------------------------------------------------------------------------------------------------------




// ATGB_Parser
int ATGB_Parser::Parse_Response_To_Message(const AString &response_text, SMessage &out_message)
{
    int last_update_id = 0;

    try  // Parse JSON from our AString
    {
        nlohmann::json json_data = nlohmann::json::parse(response_text.Get_C_Str() );

        for(auto const &item : json_data["result"])
        {
            last_update_id = item["update_id"];

            if(item.contains("message") == true)
            {
                Parse_Json(out_message, item["message"]);  // All fields, including AString, are filled automatically.

                std::println("Parsed Message: [{}] says: {}", out_message.From.First_Name.Get_C_Str(), out_message.Text.Get_C_Str() );
            }
            else if(item.contains("callback_query") == true )  // !!! TEMP
            {
                std::string pretty_json = json_data.dump(4);
                std::println("Received JSON:\n{}", pretty_json);
            }
        }
    }
    catch(const nlohmann::json::exception &e)
    {
        std::println("JSON Error: {}", e.what() );
    }

    return last_update_id;
}
//------------------------------------------------------------------------------------------------------------
double ATGB_Parser::Parse_NBU_USD_Rate(const AString &response_text, SExchange_Rate &exchange_rate) const
{
    double usd_rate;
    nlohmann::json json_data;

    usd_rate = 0.0;
    json_data = nlohmann::json::parse(response_text.Get_C_Str() );
    
    Print_Json(response_text);
    Parse_Json(exchange_rate, json_data[0]);  // use reflection to parse entire object

    if( (json_data.is_array() == true) && (json_data.empty() == false) )
        usd_rate = json_data[0]["rate"].get<double>();  // Extract rate field from the first object in the array

    return usd_rate;
}
//------------------------------------------------------------------------------------------------------------
int ATGB_Parser::Parse_Updates(const AString &response_text, SUpdate &out_updates)
{
    int last_update_id = 0;

    try  // Parse raw string into JSON object.
    {
        nlohmann::json json_data = nlohmann::json::parse(response_text.Get_C_Str() );

        for(auto const &item : json_data["result"])  // Iterate through all received updates.
        {
            SUpdate current_update;

            Parse_Json(current_update, item);  // use reflection to parse entire object

            last_update_id = current_update.Update_Id;  // Update the highest ID to acknowledge messages later.
            
            if(current_update.Callback_Query.Id.Get_Size() > 0)  // Determine the type of update and log it.
            {
                const char *first_name = current_update.Callback_Query.From.First_Name.Get_C_Str();
                const char *action_data = current_update.Callback_Query.Data.Get_C_Str();

                std::println("Button Clicked! User: [{}], Action Data: {}", first_name, action_data);
            }
            else if(current_update.Message.Text.Get_Size() > 0)
            {
                const char *first_name = current_update.Message.From.First_Name.Get_C_Str();
                const char *text = current_update.Message.Text.Get_C_Str();

                std::println("Parsed Message: [{}] says: {}", first_name, text);
            }

            out_updates = current_update;  // Store parsed update.
        }
    }
    catch(const nlohmann::json::exception &e)
    {
        std::println("JSON Error: {}", e.what() );
    }

    return last_update_id;
}
//------------------------------------------------------------------------------------------------------------
void ATGB_Parser::Print_Json(const AString &response_text) const
{
    nlohmann::json json_data;

    json_data = nlohmann::json::parse(response_text.Get_C_Str() );

    std::string pretty_json = json_data.dump(4);
    std::println("Received JSON:\n{}", pretty_json);    
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
/*
Received JSON:
{
    "ok": true,
    "result": [
        {
            "callback_query": {
                "chat_instance": "-9129611791160574222",
                "data": "action_accept",
                "from": {
                    "first_name": "Andrey",
                    "id": 1775156303,
                    "is_bot": false,
                    "language_code": "en",
                    "username": "GetBoros"
                },
                "id": "7624238266802005305",
                "message": {
                    "chat": {
                        "id": -1003763586627,
                        "is_forum": true,
                        "title": "Diary",
                        "type": "supergroup"
                    },
                    "date": 1782055956,
                    "from": {
                        "first_name": "GetBorosCppBot",
                        "id": 8830860851,
                        "is_bot": true,
                        "username": "get_boros_cpp_bot"
                    },
                    "is_topic_message": true,
                    "message_id": 78,
                    "message_thread_id": 17,
                    "reply_markup": {
                        "inline_keyboard": [
                            [
                                {
                                    "callback_data": "action_accept",
                                    "text": "Accept"
                                },
                                {
                                    "callback_data": "action_decline",
                                    "text": "Decline"
                                }
                            ]
                        ]
                    },
                    "reply_to_message": {
                        "chat": {
                            "id": -1003763586627,
                            "is_forum": true,
                            "title": "Diary",
                            "type": "supergroup"
                        },
                        "date": 1782055956,
                        "from": {
                            "first_name": "Andrey",
                            "id": 1775156303,
                            "is_bot": false,
                            "language_code": "en",
                            "username": "GetBoros"
                        },
                        "is_topic_message": true,
                        "message_id": 77,
                        "message_thread_id": 17,
                        "text": "Hi"
                    },
                    "text": "Message Handled Successfully!"
                }
            },
            "update_id": 875409095
        }
    ]
}
*/
//------------------------------------------------------------------------------------------------------------
