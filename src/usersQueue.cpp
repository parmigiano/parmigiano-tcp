#include "../include/UsersQueue.h"

UsersQueue::UsersQueue(std::string req, SOCKET sock) : request(req), clientSocket(sock) {}

usQu connectionQueue;