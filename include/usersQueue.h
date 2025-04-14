#ifndef USERSQUEUE_H
#define USERSQUEUE_H

#include <string>
#include <WinSock2.h>
#include <list>

class UsersQueue {
public:
    std::string request;
    SOCKET clientSocket;

    UsersQueue(std::string req, SOCKET sock);
};

typedef std::list<UsersQueue> usQu;
extern usQu connectionQueue;

#endif 