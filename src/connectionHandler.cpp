#include "../include/connectionHandler.h"
#include "../include/UsersQueue.h"
#include "../include/connect.h"
#include "../include/requestProcessing.h"

#include <iostream>
#include <mutex>

SOCKET clientSocket = INVALID_SOCKET;

void ConnectionHandler::addUserToQueue(string request, SOCKET socket){
    mutex addUserToQueueMutex;
    std::cout << "addUserToQueue\n";

    unique_lock<mutex> lock(addUserToQueueMutex);
    connectionQueue.push_back({ request, socket });
    lock.unlock();

    if (connectionQueue.empty()) {
        std::cout << "connectionQueue empty\n";
    }
}

void ConnectionHandler::queueHandler(){
    requestProcessing _requestProcessing;

    mutex queueMutex;

    std::cout << "queueHandler\n";

    while (true) {
        if (connectionQueue.empty()) {
            continue;
        }

        unique_lock<mutex> lock(queueMutex);
        usQu::iterator it = connectionQueue.begin();

        while (it != connectionQueue.end()) {
            //std::cout << it->clientSocket << std::endl << it->request << std::endl;
            clientSocket = it->clientSocket;
            _requestProcessing.requestDistribution(it->request); //
            it = connectionQueue.erase(it);
        }

        lock.unlock();
    }
}

void ConnectionHandler::incomingConnections(){
    Connection _Connection;

    SOCKET listenSocket = _Connection.getCurrentListenSocket();
    SOCKET clientSocketIncoming = INVALID_SOCKET;

    char recvBuffer[1024];

    while (true) {
        memset(recvBuffer, 0, sizeof(recvBuffer));

        if ((clientSocketIncoming = accept(listenSocket, NULL, NULL)) == INVALID_SOCKET) {
            std::cout << "(!) accept error\n";  
            continue;
        }

        /*addUserToQueue("", clientSocketIncoming);*/


        if (recv(clientSocketIncoming, recvBuffer, sizeof(recvBuffer), 0) > 0) {
            //string request = recvBuffer;
            std::cout << "Client has connected\n";

            //std::cout << recvBuffer << std::endl;

            addUserToQueue(recvBuffer, clientSocketIncoming);
        }
        /*else {
            addUserToQueue("", clientSocketIncoming);
        }*/
    }
}

SOCKET ConnectionHandler::getCurrentConnectClientSocket(){
    return clientSocket;
}
