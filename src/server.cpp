#include "../include/server.h"

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sstream>
#include <thread>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

Server::Server(int port) {
    this->port = port;
    this->serverSocket = -1;
    table.loadFromFile(SNAPSHOT_FILE);
}

void Server::start() {
    setupSocket();
    bindSocket();
    listenForConnections();
}

void Server::setupSocket() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET for IPv4, SOCK_STREAM for TCP

    if(serverSocket < 0) {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    int opt = 1;

    if(setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Initialize the address structure
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    std::cout << "Socket created successfully!\n";
}

void Server::bindSocket() {

    if(bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Socket bound successfully!\n";
}

void Server::listenForConnections() {
    int backlog = 5; // Number of pending connections allowed
    
    if(listen(serverSocket, backlog) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port "<< port << "..." << std::endl;

    sockaddr_in clientAddress;
    socklen_t clientLength = sizeof(clientAddress);

    while(true) {
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddress, &clientLength);

        if(clientSocket < 0) {
            perror("Accept failed");
            continue;
        }

        std::cout << "Client connected from " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << std::endl;

        std::thread clientThread(&Server::handleClient, this, clientSocket);

        clientThread.detach();
    }
}

void Server::handleClient(int clientSocket) {
    int buffer_size = 1024;

    while(true) {
        char buffer[buffer_size];

        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if(bytesReceived < 0) {
            perror("Receive failed");
            break;
        }

        if(bytesReceived == 0) {
            std::cout << "Client disconnected\n";
            break;
        }
        
        std::string command(buffer, bytesReceived);
        std::cout << "Received: " << command << std::endl;

        std::string response = processCommand(command);
        
        int byteSent = send(clientSocket, response.c_str(), response.size(), 0);

        if(response == "_EXIT_") {
            std::cout << "Exiting server..." << std::endl;
            break;
        }
        if(byteSent < 0) {
            perror("Send failed");
            break;
        }
    }

    close(clientSocket);
}

std::string Server::processCommand(const std::string& command) {
    std::string response = "Command received: " + command;

    std::stringstream ss(command);

    std::string operation;
    std::string key;
    std::string value;

    ss >> operation;

    if(operation == "EXIT") {
        response = "_EXIT_";
    }
    else if(operation == "SET") {
        ss >> key >> value;
        std::lock_guard<std::mutex> lock(tableMutex);
        table.set(key, value);
        table.saveToFile("snapshot.db");
        response = "OK";
    }
    else if(operation == "GET") {
        ss >> key;
        std::lock_guard<std::mutex> lock(tableMutex);
        std::string retrievedValue = table.get(key);
            
        if(!retrievedValue.empty()) {
            response = "Retrieved value: " + retrievedValue;
        } 
        else {
            response = "Key not found";
        }
    }
    else if(operation == "EXISTS") {
        ss >> key;
        response = table.exists(key) ? "1" : "0";
    }
    else if(operation == "DEL") {
        ss >> key;
        std::lock_guard<std::mutex> lock(tableMutex);
        table.remove(key);
        table.saveToFile("snapshot.db");
        response = "Deleted";
    }
    else {
        response = "Unknown command";
    }

    return response + "\n";
}