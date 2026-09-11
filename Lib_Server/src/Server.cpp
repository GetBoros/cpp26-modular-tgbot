//------------------------------------------------------------------------------------------------------------
module;
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
module Lib_Server;
//------------------------------------------------------------------------------------------------------------
import std;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
void Run_Ngrok()
{
    int exit_code;
    const char *command_c_str;

    // 1.0. Terminate old ngrok instances and launch new detached process
    command_c_str = 
        "killall -q ngrok; "
        "ngrok http 8080 --url https://unharmed-encore-accustom.ngrok-free.dev > /dev/null 2>&1 &";

    // 1.1. Execute shell command
    exit_code = std::system(command_c_str);
    if(exit_code == 0)
        std::println("ngrok tunnel launched in background.");
}
//------------------------------------------------------------------------------------------------------------
void Run_Server()
{
    bool is_options;
    int opt;
    int port;
    int server_fd;
    int client_fd;
    int session_counter;
    socklen_t addr_len;
    long long bytes_read;
    struct sockaddr_in address;
    std::string request_str;
    std::string response_body;
    std::string http_response;
    char buffer[2048];

    // 1.0. Prepare network configuration
    opt = 1;
    port = 8080;
    session_counter = 0;
    addr_len = sizeof(address);
    std::memset(&address, 0, sizeof(address));

    // 1.1. Create socket descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1)
    {
        std::println("Failed to create server socket");
        return;
    }

    // 1.2. Configure socket options
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // 1.3. Bind socket to target port
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::println("Bind failed on port {}", port);
        close(server_fd);
        return;
    }

    // 1.4. Start listening for incoming connections
    if(listen(server_fd, 10) < 0)
    {
        std::println("Listen failed");
        close(server_fd);
        return;
    }

    std::println("C++26 Game Server started on port {}...", port);

    // 2.0. Infinite server request loop
    while(true)
    {
        std::memset(buffer, 0, sizeof(buffer));

        // 2.1. Wait for incoming connection
        client_fd = accept(server_fd, (struct sockaddr *)&address, &addr_len);
        if(client_fd < 0)
        {
            std::println("Accept failed");
            continue;
        }

        // 2.2. Read incoming request data
        bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
        if(bytes_read <= 0)
        {
            close(client_fd);
            continue;
        }

        request_str = std::string(buffer, bytes_read);

        // 2.3. Handle CORS Preflight (OPTIONS request)
        is_options = (request_str.starts_with("OPTIONS") == true);
        if(is_options == true)
        {
            http_response = 
                "HTTP/1.1 200 OK\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n"
                "Access-Control-Allow-Headers: Content-Type, Authorization\r\n"
                "Content-Length: 0\r\n"
                "Connection: close\r\n\r\n";

            send(client_fd, http_response.c_str(), http_response.size(), 0);
            close(client_fd);
            continue;
        }

        // 2.4. Route: POST /login
        if(request_str.find("POST /login") != std::string::npos)
        {
            session_counter++;
            const std::string session_token = "dao_session_token_" + std::to_string(session_counter);

            std::println("--> [AUTH] User login request received! Issued Token: {}", session_token);

            response_body = 
                "{\"status\":\"success\","
                "\"token\":\"" + session_token + "\","
                "\"message\":\"Welcome to Cultivation Realm!\"}\n";

            http_response = 
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n"
                "Access-Control-Allow-Headers: Content-Type, Authorization\r\n"
                "Content-Length: " + std::to_string(response_body.size()) + "\r\n"
                "Connection: close\r\n\r\n" + response_body;

            send(client_fd, http_response.c_str(), http_response.size(), 0);
            close(client_fd);
            continue;
        }

        // 2.5. Fallback 404 Route
        response_body = "{\"status\":\"error\",\"message\":\"Endpoint not found\"}\n";
        http_response = 
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: application/json\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Content-Length: " + std::to_string(response_body.size()) + "\r\n"
            "Connection: close\r\n\r\n" + response_body;

        send(client_fd, http_response.c_str(), http_response.size(), 0);
        close(client_fd);
    }

    // 3.0. Release main server socket
    close(server_fd);
}
//------------------------------------------------------------------------------------------------------------
void Handle_Lib_Server()
{// ngrok http 8080 --url https://unharmed-encore-accustom.ngrok-free.dev || http://localhost:4040/inspect/http debug
    
    Run_Ngrok();
    Run_Server();
};
//------------------------------------------------------------------------------------------------------------
