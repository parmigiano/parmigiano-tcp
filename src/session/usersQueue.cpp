#include "session/usersQueue.h"

#include "connect_processing/requestProcessing.h"
#include "session/session.h"

//#include <iostream>

UsersQueue* UsersQueue::instance_ptr_ = nullptr;
std::mutex UsersQueue::instance_mtx_;

std::mutex qmutex;
std::condition_variable cv;
std::queue<ConnectionInfoFields> connections;

UsersQueue::UsersQueue() {
    //_RequestProcessing = RequestProcessing::get_instance();

    _RequestProcessing = std::make_shared<RequestProcessing>();
}

UsersQueue* UsersQueue::get_instance() {
    std::lock_guard<std::mutex> lock(instance_mtx_);
    if (instance_ptr_ == nullptr) {
        instance_ptr_ = new UsersQueue();
    }

    return instance_ptr_;
}

void UsersQueue::addUserToQueue(std::string& request, Session& session) {
    std::lock_guard<std::mutex> lock(qmutex);
    connections.push( {request, session} );
    cv.notify_one();
}

void UsersQueue::queueHandler() {
    std::unique_lock<std::mutex> lock(qmutex);

    while (true) {
        cv.wait(lock, [this] {
            return !connections.empty(); 
        });

        ConnectionInfoFields requestInfo = connections.front();
        connections.pop();

        //std::cout << "new user" << std::endl;

        lock.unlock();

        _RequestProcessing->requestDistribution(requestInfo.request, requestInfo.session);

        lock.lock();
    }
}