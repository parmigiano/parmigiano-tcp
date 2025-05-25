#ifndef CONNECTION_H
#define CONNECTION_H

#include <WinSock2.h>
#include <WS2tcpip.h>

#define WIN32_LEAN_AND_MEAN
#pragma comment(lib, "WS2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

class Connection {
private:
public:
	void createConnection(const char* PORT);
	fd_set getMasterSet();
	SOCKET getListenSocket();
};

#endif 
