#include "../include/connectionHandler.h"
#include "../include/connect.h"
#include "../include/requestProcessing.h"

#include <iostream>
#include <queue>
#include <utility>
#include <mutex>

std::mutex queueMutex;

SOCKET clientSocket = INVALID_SOCKET;
std::queue<std::pair<std::string, SOCKET>> usersQueue;

ConnectionHandler::ConnectionHandler(){
    _Logger = Logger::get_instance();

    _RequestProcessing = std::make_shared<RequestProcessing>();
    _Connection = std::make_shared <Connection>();
}

void ConnectionHandler::addUserToQueue(std::string request, SOCKET socket){
    std::lock_guard<std::mutex> lock(queueMutex);
    usersQueue.push({ request, socket });
}

void ConnectionHandler::queueHandler(){

    while (true) {

        if (usersQueue.empty()) {
            continue;
        }

        std::lock_guard<std::mutex> lock(queueMutex);

        std::pair<std::string, SOCKET> front = usersQueue.front();

        clientSocket = front.second;
        _RequestProcessing->requestDistribution(front.first, front.second);
        
        usersQueue.pop();
    }
}

void ConnectionHandler::incomingConnections(){
    SOCKET listenSocket = _Connection->getListenSocket();
    fd_set masterSet = _Connection->getMasterSet();
    SOCKET clientSocketIncoming = INVALID_SOCKET;

    int socketCount = 0;

    char recvBuffer[1024];

    while (true) {
        fd_set copySet = masterSet;
        timeval timeout;

        timeout.tv_sec = 60;
        timeout.tv_usec = 0;

        socketCount = select(0, &copySet, nullptr, nullptr, &timeout);

        if (socketCount == 0) {
            _Logger->addLog("INFO", "Timeout: haven't new connections", 2);
            continue;
        }

        for (int i = 0; i < socketCount; i++) {
            SOCKET fd_sock = copySet.fd_array[i];

            memset(recvBuffer, 0, sizeof(recvBuffer));

            if (fd_sock == listenSocket) {clientSocketIncoming = accept(listenSocket, NULL, NULL);
                FD_SET(clientSocketIncoming, &masterSet);
                _Logger->addLog("INFO", "Client connected", 2);
                continue;
            }

            int bytesIn = recv(fd_sock, recvBuffer, sizeof(recvBuffer), 0);

            if (bytesIn <= 0) {
                closesocket(fd_sock);
                FD_CLR(fd_sock, &masterSet);
                _Logger->addLog("INFO", "Client disconnected", 2);
            }
            else {
                addUserToQueue(recvBuffer, fd_sock);
            }
            
        }

    }
}

SOCKET ConnectionHandler::getCurrentConnectClientSocket(){
    return clientSocket;
}