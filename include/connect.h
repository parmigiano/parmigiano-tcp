#ifndef CONNECTION_H
#define CONNECTION_H

#include <WinSock2.h>
#include <WS2tcpip.h>

#include "../include/logger.h"

#define WIN32_LEAN_AND_MEAN
#pragma comment(lib, "WS2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

class Connection {
private:
	Logger* _Logger;
public:
	Connection();
	~Connection() = default;

	void createConnection(const char* PORT);
	fd_set getMasterSet();
	SOCKET getListenSocket();
};

//extern Connection _Connection;

#endif 