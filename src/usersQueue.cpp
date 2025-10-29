#include "../include/usersQueue.h"

#include "../include/requestProcessing.h"

std::mutex qmutex;
std::condition_variable cv;
std::queue<ConnectionInfoFields> connections;

UsersQueue::UsersQueue() {
    _RequestProcessing = std::make_shared<RequestProcessing>();
}

void UsersQueue::addUserToQueue(std::string request, boost::asio::ip::tcp::socket& socket) {
    std::lock_guard<std::mutex> lock(qmutex);
    connections.push( {request, socket} );
    cv.notify_one();
}

void UsersQueue::queueHandler() {
    std::unique_lock<std::mutex> lock(qmutex);

    while (true) {
        cv.wait(lock, [this] {return !connections.empty(); });

        ConnectionInfoFields requestInfo = connections.front();
        connections.pop();

        lock.unlock();

        _RequestProcessing->requestDistribution(requestInfo.request, requestInfo.socket);

        lock.lock();
    }
}