#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error: Failed to initialize Winsock." << std::endl;
        return -1;
    }

    // Create socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error: Failed to create socket." << std::endl;
        WSACleanup();
        return -1;
    }

    // Connect to server
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddress.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Error: Failed to connect to server." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Connected to server" << std::endl;

    // Send message to server
    std::cout << "Enter the message you want to send: ";
    std::string message;
    std::getline(std::cin, message);

    send(clientSocket, message.c_str(), message.length(), 0);

    // Close socket
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
