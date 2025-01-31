# Servidor i Client TCP en C++

Aquest projecte implementa un **servidor** i un **client** TCP en C++ per gestionar comandes de clients i desar-les en un fitxer de text.

## 📌 Característiques
- Servidor multi-fil per gestionar múltiples clients simultàniament.
- Guarda les comandes rebudes en un fitxer `comandes.txt`.
- Generació automàtica d'ID de comandes.
- Compatible amb **Windows** i **Linux**.

---

## 🛠️ Requeriments

### 🔹 Windows
- MinGW (per a `g++`) o Microsoft Visual Studio.

### 🔹 Linux
- `g++`
- `pthread` (per als fils)

---

## 🚀 Instal·lació i Execució

### 🔹 1️⃣ Compilar el codi

#### **Windows (MinGW)**
```sh
 g++ server.cpp -o server.exe -lws2_32 -pthread
 g++ client.cpp -o client.exe -lws2_32
```

#### **Linux**
```sh
 g++ server.cpp -o server -pthread
 g++ client.cpp -o client
```

### 🔹 2️⃣ Executar el servidor
```sh
./server  # (Linux)
server.exe  # (Windows)
```

### 🔹 3️⃣ Executar el client
```sh
./client  # (Linux)
client.exe  # (Windows)
```

Quan el client enviï una comanda, el servidor la registrarà a `comandes.txt` amb un identificador únic.

---

## 🔄 Exemple de Funcionament

### **1️⃣ El client introdueix una comanda:**
```sh
>> Connexió establerta amb el servei.
>> Introduïu la vostra comanda: Pizza de 4 formatges
>> Comanda enviada al servei.
>> Identificador de la comanda: ORD-0001
```

### **2️⃣ El servidor guarda la comanda:**
```sh
>> Servei iniciat. Escoltant al port 9100...
>> Connexió acceptada d'un client.
```
📄 **comandes.txt:**
```
ORD-0001: "Pizza de 4 formatges"
```

---

## 📝 Llicència
Aquest projecte està sota la llicència MIT. Pots modificar-lo i utilitzar-lo lliurement. 🚀

