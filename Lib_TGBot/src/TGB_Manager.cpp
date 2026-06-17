//------------------------------------------------------------------------------------------------------------
module;
#include <meta>
#include <iostream>
#include <concepts>
module TGB_Manager;
//------------------------------------------------------------------------------------------------------------
import TGB_Parser;
import TGB_Network;
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
template <typename T> concept Printable = requires(std::ostream& os, const T& value)
{
    os << value;
};

// Универсальная функция для инспекции любого объекта
template <typename T> void inspect_class(const T& obj)
{
    constexpr std::meta::info type_meta = ^^T;

    std::cout << "==================================================\n";
    std::cout << "АНАЛИЗ КЛАССА: " << std::meta::display_string_of(type_meta) << "\n";
    std::cout << "==================================================\n";

    // --- СЕКЦИЯ 1: ПЕРЕМЕННЫЕ (ПОЛЯ) ---
    std::cout << "[Переменные (Поля)]:\n";
    template for (constexpr auto member : std::define_static_array(
        std::meta::nonstatic_data_members_of(type_meta, std::meta::access_context::unchecked())
    ))
    {
        constexpr auto name = std::meta::identifier_of(member);
        constexpr auto type_meta = std::meta::type_of(member);
        constexpr auto type_name = std::meta::display_string_of(type_meta);

        // Получаем тип конкретного поля у объекта
        using FieldType = decltype(obj.[:member:]);

        std::cout << "  - " << type_name << " " << name;

        // Безопасный вывод значения: если тип можно напечатать, печатаем. 
        // Иначе пишем, что тип не поддерживает вывод.
        if constexpr (Printable<FieldType>) {
            std::cout << " = " << obj.[:member:] << "\n";
        } else {
            std::cout << " = [unprintable type]\n";
        }
    }

    std::cout << "\n";

    // --- СЕКЦИЯ 2: МЕТОДЫ (ФУНКЦИИ) ---
    std::cout << "[Методы (Функции)]:\n";
    template for (constexpr auto member : std::define_static_array(
        std::meta::members_of(type_meta, std::meta::access_context::unchecked())
    )) {
        if constexpr (std::meta::is_function(member) 
                      && std::meta::has_identifier(member)
                      && !std::meta::is_constructor(member) 
                      && !std::meta::is_destructor(member)) {
            
            constexpr auto method_name = std::meta::identifier_of(member);
            constexpr auto ret_type = std::meta::return_type_of(member);
            constexpr auto ret_name = std::meta::display_string_of(ret_type);

            std::cout << "  - " << ret_name << " " << method_name << "(";

            bool first = true;
            template for (constexpr auto param : std::define_static_array(std::meta::parameters_of(member))) {
                if (!first) std::cout << ", ";
                first = false;

                constexpr auto param_type = std::meta::type_of(param);
                constexpr auto param_type_name = std::meta::display_string_of(param_type);
                
                if constexpr (std::meta::has_identifier(param) )
                {
                    constexpr auto param_name = std::meta::identifier_of(param);
                    
                    std::cout << param_type_name << " " << param_name;
                }
                else
                {
                    std::cout << param_type_name;
                }
            }
            std::cout << ")\n";
        }
    }
    std::cout << "==================================================\n\n";
}
//------------------------------------------------------------------------------------------------------------




// ATGB_Manager
ATGB_Manager::ATGB_Manager()
{

}
//------------------------------------------------------------------------------------------------------------
void ATGB_Manager::Initialize()
{
    int last_update_id;

    ATGB_Parser parser;
    ATGB_Network network;

    last_update_id = 0;
    
    inspect_class(parser);

    while(true)
    {
        long long id_chat, id_chat_topic;
        const char *response = network.Get_Response(last_update_id);

        last_update_id = parser.Set_Response_Text(response, id_chat, id_chat_topic);
        if(id_chat != 0)  // Example chat_id and message
            network.Send_Message(id_chat, id_chat_topic, "Message Handled Successfully!");

    }

}
//------------------------------------------------------------------------------------------------------------
