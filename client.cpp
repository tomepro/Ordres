#include <iostream>     // Per entrada i sortida (cin, cout)
#include <string>       // Per gestionar cadenes de text (std::string)
#include <winsock2.h>   // Llibreria per a sockets a Windows
#include <ws2tcpip.h>   // Funcions addicionals per sockets a Windows

#pragma comment(lib, "ws2_32.lib")  // Enllaça automàticament la llibreria de sockets

using namespace std;

int main() {
    WSADATA wsaData;  
    WSAStartup(MAKEWORD(2, 2), &wsaData);  // Inicialitza Winsock (necessari per a sockets a Windows)

    // Crea el socket del client
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Error creant el socket del client." << endl;
        return 1;
    }

    // Configura l'adreça del servidor
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;       // Utilitza IPv4
    serverAddr.sin_port = htons(9100);     // Defineix el port 9100
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);  // Converteix l'adreça IP

    // Connecta el client al servidor
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "No es pot connectar al servidor." << endl;
        return 1;
    }

    cout << ">> Connexio establerta amb el servei." << endl;
    cout << ">> Introduiu la vostra comanda: ";
    
    string order;
    getline(cin, order);  // Llegeix la comanda introduïda per l'usuari

    // Envia la comanda al servidor
    send(clientSocket, order.c_str(), order.size(), 0);
    cout << ">> Comanda enviada al servei." << endl;

    // Rep la resposta del servidor
    char buffer[1024] = {0};  // Buffer per emmagatzemar la resposta
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0) {
        cout << ">> " << buffer << endl;  // Mostra la resposta (ID de la comanda)
    }

    // Tanca la connexió amb el servidor
    closesocket(clientSocket);
    WSACleanup();  // Neteja i desregistra l'ús de Winsock
    return 0;
}
