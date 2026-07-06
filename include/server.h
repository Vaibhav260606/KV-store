#ifndef SERVER_H
#define SERVER_H

#include "hashtable.h"
#include <netinet/in.h>
#include <mutex>
#include <thread>

class Server {
public:
    Server(int port);
    void start();

private:
    void setupSocket();
    void bindSocket();
    void listenForConnections();
    void handleClient(int clientSocket);
    
    std::string processCommand(const std::string& command);

    int port;
    int serverSocket;

    sockaddr_in serverAddress;

    static constexpr const char* SNAPSHOT_FILE = "snapshot.db";

    HashTable table;

    std::mutex tableMutex;
};

#endif