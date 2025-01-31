#include <iostream>     // Entrada i sortida
#include <fstream>      // Gestió d'arxius
#include <string>       // Manipulació de cadenes
#include <thread>       // Execució de fils
#include <mutex>        // Sincronització d'accessos al fitxer
#include <vector>       // (Opcional) Emmagatzematge de dades
#include <sstream>      // Manipulació de text en fluxos
#include <iomanip>      // Formatació de text
#include <winsock2.h>   // Llibreria de sockets per Windows
#include <ws2tcpip.h>   // Funcions addicionals per sockets

#pragma comment(lib, "ws2_32.lib")  // Enllaça la llibreria de sockets

using namespace std;

mutex fileMutex;  // Mutex per evitar problemes en accés simultani al fitxer
int orderCounter = 1;  // Contador d'ID de comandes

// Funció per generar un identificador únic de comanda
string generateOrderID() {
    lock_guard<mutex> lock(fileMutex);  // Bloqueja l'accés a la variable compartida
    stringstream ss;
    ss << "ORD-" << setw(4) << setfill('0') << orderCounter++;  // Format: ORD-0001, ORD-0002...
    return ss.str();
}

// Funció per gestionar un client en un fil separat
void handleClient(SOCKET clientSocket) {
    char buffer[1024] = {0};  // Buffer per rebre la comanda
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);  // Rep la comanda del client
    
    if (bytesRead > 0) {  
        string order(buffer);  // Converteix el buffer a string
        string orderID = generateOrderID();  // Genera l'ID de comanda

        // Desa la comanda al fitxer `comandes.txt`
        {
            lock_guard<mutex> lock(fileMutex);  // Bloqueja l'accés al fitxer
            ofstream file("comandes.txt", ios::app);  // Obre el fitxer en mode append
            if (file.is_open()) {
                file << orderID << ": \"" << order << "\"\n";  // Desa la comanda
                file.close();
            }
        }

        // Respon al client amb l'ID de la comanda
        string response = "Identificador de la comanda: " + orderID + "\n";
        send(clientSocket, response.c_str(), response.size(), 0);
    }

    closesocket(clientSocket);  // Tanca la connexió amb el client
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);  // Inicialitza la llibreria Winsock

    // Creació del socket del servidor
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Error creant el socket del servidor." << endl;
        return 1;
    }

    // Configura l'adreça del servidor
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;       // IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Accepta connexions de qualsevol IP
    serverAddr.sin_port = htons(9100);     // Port 9100

    // Vincula el socket al port especificat
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Error vinculant el socket al port." << endl;
        return 1;
    }

    // Posa el socket en mode escolta (fins a 5 connexions en cua)
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        cerr << "Error escoltant connexions." << endl;
        return 1;
    }

    cout << ">> Servei iniciat. Escoltant al port 9100..." << endl;

    // Bucle principal per acceptar connexions de clients
    while (true) {
        sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);

        if (clientSocket == INVALID_SOCKET) {
            cerr << "Error acceptant connexió." << endl;
            continue;
        }

        cout << ">> Connexió acceptada d'un client." << endl;

        // Crea un nou fil per atendre el client i el deslliga (detach)
        thread clientThread(handleClient, clientSocket);
        clientThread.detach();  // Permet que el fil segueixi executant-se de manera independent
    }

    closesocket(serverSocket);  // Tanca el socket del servidor
    WSACleanup();  // Neteja l'ús de Winsock
    return 0;
}

