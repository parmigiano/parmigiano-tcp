#include "../include/usersQueue.h"

#include "../include/requestProcessing.h"

#include <mutex>
#include <iostream>

std::mutex mutex;
std::queue<ConnectionInfoFields> connections;

UsersQueue::UsersQueue() {
    _RequestProcessing = std::make_shared<RequestProcessing>();
}

void UsersQueue::addUserToQueue(std::string request, boost::asio::ip::tcp::socket& socket) {
    std::lock_guard<std::mutex> lock(mutex);
    connections.push( {request, socket} );
}

void UsersQueue::queueHandler() {
    while (true) {

        if (connections.empty()) {
            continue;
        }

        std::lock_guard<std::mutex> lock(mutex);

        ConnectionInfoFields requestInfo = connections.front();

        _RequestProcessing->requestDistribution(requestInfo.request, requestInfo.socket);

        connections.pop();
    }
}