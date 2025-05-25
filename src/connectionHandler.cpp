#include "../include/connectionHandler.h"
#include "../include/UsersQueue.h"
#include "../include/connect.h"
#include "../include/requestProcessing.h"
#include "../include/logger.h"

#include <iostream>
#include <mutex>

SOCKET clientSocket = INVALID_SOCKET;

void ConnectionHandler::addUserToQueue(string request, SOCKET socket){
    mutex addUserToQueueMutex;
    //_Logger.addLog("INFO", "Add user to queue", 1);

    unique_lock<mutex> lock(addUserToQueueMutex);
    connectionQueue.push_back({ request, socket });
    lock.unlock();

    if (connectionQueue.empty()) {
        _Logger.addLog("PROBLEM", "connection queue is empty", 1);
    }
}

void ConnectionHandler::queueHandler(){
    requestProcessing _requestProcessing;

    mutex queueMutex;

    //std::cout << "queueHandler\n";

    while (true) {
        if (connectionQueue.empty()) {
            continue;
        }

        unique_lock<mutex> lock(queueMutex);
        usQu::iterator it = connectionQueue.begin();

        while (it != connectionQueue.end()) {  // replace to STL queue
            //std::cout << it->clientSocket << std::endl << it->request << std::endl;
            clientSocket = it->clientSocket;
            _requestProcessing.requestDistribution(it->request); //
            it = connectionQueue.erase(it);

            // std::cout << "Received: " << it->request << std::endl;
        }

        lock.unlock();
    }
}

void ConnectionHandler::incomingConnections(){
    Connection _Connection;

    SOCKET listenSocket = _Connection.getListenSocket();
    fd_set masterSet = _Connection.getMasterSet();
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
            _Logger.addLog("INFO", "Timeout: haven't new connections", 1);
            continue;
        }

        for (int i = 0; i < socketCount; i++) {
            SOCKET fd_sock = copySet.fd_array[i];

            memset(recvBuffer, 0, sizeof(recvBuffer));

            if (fd_sock == listenSocket) {clientSocketIncoming = accept(listenSocket, NULL, NULL);
                FD_SET(clientSocketIncoming, &masterSet);
                //std::cout << "Client connected" << std::endl;
                continue;
            }

            int bytesIn = recv(fd_sock, recvBuffer, sizeof(recvBuffer), 0);

            if (bytesIn <= 0) {
                closesocket(fd_sock);
                FD_CLR(fd_sock, &masterSet);
                //std::cout << "Client disconnected" << std::endl;
            }
            else {
                addUserToQueue(recvBuffer, fd_sock);
                //recvBuffer[bytesIn] = '\0';
                //std::cout << "Received: " << recvBuffer << std::endl;
            }
            
        }

    }
}

SOCKET ConnectionHandler::getCurrentConnectClientSocket(){
    return clientSocket;
}