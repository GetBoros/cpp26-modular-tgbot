//------------------------------------------------------------------------------------------------------------
module;
#include <ixwebsocket/IXWebSocket.h>
#include <iostream>
#include <string>
module Lib_CBWatcher;
//------------------------------------------------------------------------------------------------------------
import std;
import ACB_Fetcher;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
std::string get_field(const std::string& str, const std::string& key) {
    // 1. Ищем только после блока "data", чтобы не цеплять "messages":[{ из шапки
    size_t start_pos = 0;
    auto data_pos = str.find("\"data\"");
    if (data_pos != std::string::npos) {
        start_pos = data_pos;
    }

    // 2. Ищем ключ в кавычках: например \"message\" или "message"
    std::string target = "\"" + key + "\"";
    auto pos = str.find(target, start_pos);
    if (pos == std::string::npos) {
        pos = str.find(key, start_pos);
        if (pos == std::string::npos) return "";
    }

    pos = str.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;

    // Пропускаем двоеточие, пробелы, кавычки и обратные слэши
    while (pos < str.size() && (str[pos] == ' ' || str[pos] == '\\' || str[pos] == '\"')) {
        pos++;
    }

    auto end = pos;
    while (end < str.size()) {
        // Конец строки значения
        if (str[end] == '\\' && end + 1 < str.size() && str[end + 1] == '\"') break;
        if (str[end] == '\"' || str[end] == ',') break;
        end++;
    }

    return str.substr(pos, end - pos);
}
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
void Handle_CBWatcher()
{
    ACB_Fetcher cb_fetcher;
    std::string access_token = cb_fetcher.Find_Access_Token("germaine_jones");

    std::cout << "[Результат шага 1]: " << access_token << "\n";

    return;

    ix::WebSocket webSocket;
    std::string url = "wss://realtime.pa.highwebmedia.com/?access_token=eyJhbGciOiJIUzI1NiIsImtpZCI6IktTS3cyZy5MMzZJU2ciLCJ0eXAiOiJKV1QifQ.eyJpYXQiOjE3ODkzODkzNDAsImV4cCI6MTc4OTQ3NTc0MC4wLCJ4LWFibHktY2FwYWJpbGl0eSI6IntcInJvb21fdXNlcjpncm91cGVkOk5LTUpZSDU6VzQwNkU0VjowXCI6IFtcInByZXNlbmNlXCIsIFwic3Vic2NyaWJlXCJdLCBcInVzZXI6Z3JvdXBlZDpXNDA2RTRWXCI6IFtcInN1YnNjcmliZVwiXSwgXCJnbG9iYWw6cHVzaF9zZXJ2aWNlXCI6IFtcInN1YnNjcmliZVwiXSwgXCJyb29tOmdyb3VwZWQ6TktNSllINTowXCI6IFtcInN1YnNjcmliZVwiXSwgXCJyb29tOmZhbmNsdWI6TktNSllINVwiOiBbXCJzdWJzY3JpYmVcIl0sIFwicm9vbTpzaG9ydGNvZGU6TktNSllINVwiOiBbXCJzdWJzY3JpYmVcIl19IiwieC1hYmx5LWNsaWVudElkIjoiK3NocTMxeG1zbGtiLVc0MDZFNFYifQ.VhUoNhwqM1_cyibezZbnpKAxiQMbShBt5GcR9H7Fv9E&format=json&heartbeats=true&v=3&agent=ably-js/2.12.0 browser&remainPresentFor=0";
    
    webSocket.setUrl(url);

    ix::WebSocketHttpHeaders headers;
    headers["User-Agent"] = "Mozilla/5.0 (X11; Linux x86_64; rv:128.0) Gecko/20100101 Firefox/128.0";
    headers["Origin"] = "https://chaturbate.com";
    webSocket.setExtraHeaders(headers);

    webSocket.setOnMessageCallback([&webSocket](const ix::WebSocketMessagePtr& msg) {
        if (msg->type == ix::WebSocketMessageType::Open) {
            std::cout << "\033[32m[+] Соединение с сервером открыто!\033[0m\n";
        }
        else if (msg->type == ix::WebSocketMessageType::Close) {
            std::cout << "\033[31m[-] Соединение закрыто сервером: " << msg->closeInfo.reason << "\033[0m\n";
        }
        else if (msg->type == ix::WebSocketMessageType::Error) {
            std::cout << "\033[31m[-] Сетевая ошибка: " << msg->errorInfo.reason << "\033[0m\n";
        }
        else if (msg->type == ix::WebSocketMessageType::Message) {

            // 1. Игнорируем пинг-понг сервера (action: 0)
            if (msg->str.find("{\"action\":0}") != std::string::npos) {
                return;
            }

            // 2. Ошибка сервера (например, токен истек)
            if (msg->str.find("\"action\":9") != std::string::npos) {
                std::cout << "\033[31m[!] Ошибка сервера: " << msg->str << "\033[0m\n";
                return;
            }

            // 3. Авторизация и подписка
            if (msg->str.find("\"action\":4") != std::string::npos) {
                std::cout << "\033[32m[+] Авторизован! Отправляю команду подписки...\033[0m\n";
                webSocket.send(R"({"action":10,"channel":"room:grouped:NKMJYH5:0","params":{},"flags":327680})");
                return;
            }

            if (msg->str.find("\"action\":11") != std::string::npos) {
                std::cout << "\033[36m[+] Подписка активна! Слушаю чат и донаты...\033[0m\n\n";
                return;
            }

            // 4. ЛОВИМ РЕАЛЬНЫЙ ТИП (ДОНАТ)
            if (msg->str.find("RoomTipAlertTopic") != std::string::npos) {
                std::string amount_str = get_field(msg->str, "amount");
                int amount = amount_str.empty() ? 0 : std::stoi(amount_str);

                std::string from = get_field(msg->str, "from_username");
                if (from.empty()) from = "Аноним";

                std::cout << "\033[1;32m[💰 ДОНАТ]: " << from 
                          << " закинул " << amount << " токенов!\033[0m\n";

                if (amount >= 500) {
                    std::cout << "\n\033[1;41;37m >>> ВНИМАНИЕ: БОЛЬШОЙ ТИП (" 
                              << amount << " токенов)! БЕГИ СМОТРЕТЬ! <<< \033[0m\a\n\n";
                }
            }
            // 5. ЛОВИМ ОБЫЧНЫЙ ЧАТ
            else if (msg->str.find("RoomMessageTopic") != std::string::npos) {
                std::string user = get_field(msg->str, "username");
                std::string text = get_field(msg->str, "message");

                if (!user.empty()) {
                    std::cout << "\033[36m[ЧАТ] " << user << ": \033[0m" << text << "\n";
                }
            }
        }
    });

    webSocket.start();

    std::cout << "Слушаю комнату... Нажми Enter для выхода.\n";
    std::cin.get();

    webSocket.stop();
}
//------------------------------------------------------------------------------------------------------------
