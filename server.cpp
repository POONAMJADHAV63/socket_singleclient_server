//SERVER
#include <iostream>
#include <fstream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error: Failed to initialize Winsock." << std::endl;
        return -1;
    }

    // Create socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error: Failed to create socket." << std::endl;
        WSACleanup();
        return -1;
    }

    // Bind socket
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Error: Failed to bind socket." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // Listen for connections
    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        std::cerr << "Error: Failed to listen for connections." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    // Accept connection
    SOCKET clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error: Failed to accept connection." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Client connected" << std::endl;

    // Receive message from client
    char messageBuffer[BUFFER_SIZE];
    int messageLength = recv(clientSocket, messageBuffer, BUFFER_SIZE, 0);
    if (messageLength == SOCKET_ERROR) {
        std::cerr << "Error: Failed to receive message." << std::endl;
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    messageBuffer[messageLength] = '\0';
    std::cout << "Message received from client: " << messageBuffer << std::endl;

    // Save message into a text file
    std::ofstream outputFile("received_message.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Error: Failed to open file for writing." << std::endl;
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    outputFile << messageBuffer;

    outputFile.close();

    std::cout << "Message saved as 'received_message.txt'" << std::endl;

    // Close sockets
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
