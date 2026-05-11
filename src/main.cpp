#include <iostream>
#include <netinet/in.h>
#include <thread>
#include <unistd.h> // close用
#include <string>
#include <cstring>



void Connect() {
    
}

void SendTask(int client_fd) {
    int c = 0;
    while (true) {
        std::string text = "server->client " + std::to_string(c++) + "\n";
        if (send(client_fd, text.c_str(), text.length(), 0) < 0) {
            break; 
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
    }
}

int main(void) {
    std::cout << "Welcome to the Virtual Pet Home!" << std::endl;
    std::cout << "Your pet is happy and healthy!" << std::endl;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 5);
    
    std::cout << "Server listening on port 8080..." << std::endl;

    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);

    std::thread thread_send(SendTask, client_fd);
    // std::thread thread_recv(RecvTask, &socket_server);

    thread_send.join();


    close(server_fd);
    return 0;
}