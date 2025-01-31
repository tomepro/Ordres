#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <sstream>
#include <iomanip>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

mutex fileMutex;
int orderCounter = 1;

string generateOrderID() {
    lock_guard<mutex> lock(fileMutex);
    stringstream ss;
    ss << "ORD-" << setw(4) << setfill('0') << orderCounter++;
    return ss.str();
}

void handleClient(SOCKET clientSocket) {
    char buffer[1024] = {0};
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    
    if (bytesRead > 0) {
        string order(buffer);
        string orderID = generateOrderID();

        {
            lock_guard<mutex> lock(fileMutex);
            ofstream file("comandes.txt", ios::app);
            if (file.is_open()) {
                file << orderID << ": \"" << order << "\"\n";
                file.close();
            }
        }

        string response = "Identificador de la comanda: " + orderID + "\n";
        send(clientSocket, response.c_str(), response.size(), 0);
    }

    closesocket(clientSocket);
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Error creant el socket del servidor." << endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(9100);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Error vinculant el socket al port." << endl;
        return 1;
    }

    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        cerr << "Error escoltant connexions." << endl;
        return 1;
    }

    cout << ">> Servei iniciat. Escoltant al port 9100..." << endl;

    while (true) {
        sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);

        if (clientSocket == INVALID_SOCKET) {
            cerr << "Error acceptant connexio." << endl;
            continue;
        }

        cout << ">> Connexio acceptada d'un client." << endl;
        thread clientThread(handleClient, clientSocket);
        clientThread.detach();
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
