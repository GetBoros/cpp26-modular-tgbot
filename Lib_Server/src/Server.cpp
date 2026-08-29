//------------------------------------------------------------------------------------------------------------
module;
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <print>
#include <string>
module Lib_Server;
//------------------------------------------------------------------------------------------------------------
import std;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
void Run_Server()
{
    int opt;
    int port;
    int server_fd;
    int client_fd;
    int total_clicks;
    socklen_t addr_len;
    long long bytes_read;
    struct sockaddr_in address;
    char buffer[1024];

    // 1.0. Prepare network configuration
    opt = 1;
    port = 8080;
    total_clicks = 0;
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

    std::println("C++26 Server started. Listening on port {}...", port);

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
        if(bytes_read > 0)
        {
            total_clicks++;
            std::println("--> Incoming Request #{} ({} bytes)", total_clicks, bytes_read);
        }

        // 2.3. Build HTTP response with updated click counter
        const std::string json_payload = "{\"status\": \"ok\", \"server\": \"C++26\", \"clicks\": " + std::to_string(total_clicks) + "}\n";
        const std::string http_response = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Content-Length: " + std::to_string(json_payload.size()) + "\r\n"
            "Connection: close\r\n\r\n" + json_payload;

        // 2.4. Send response and close ONLY the client socket
        send(client_fd, http_response.c_str(), http_response.size(), 0);
        close(client_fd);
    }

    // 3.0. Release main server socket
    close(server_fd);
}
//------------------------------------------------------------------------------------------------------------
void Handle_Lib_Server()
{
    Run_Server();

};
//------------------------------------------------------------------------------------------------------------
