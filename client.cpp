#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Error creant el socket del client." << endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9100);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "No es pot connectar al servidor." << endl;
        return 1;
    }

    cout << ">> Connexio establerta amb el servei." << endl;
    cout << ">> Introduiu la vostra comanda: ";
    string order;
    getline(cin, order);

    send(clientSocket, order.c_str(), order.size(), 0);
    cout << ">> Comanda enviada al servei." << endl;

    char buffer[1024] = {0};
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0) {
        cout << ">> " << buffer << endl;
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
