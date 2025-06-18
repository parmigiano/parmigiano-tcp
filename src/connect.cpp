#include "../include/connect.h"

#include <iostream>

//Connection _Connection;
SOCKET listenSocket = INVALID_SOCKET;
fd_set masterSet;

Connection::Connection(){
	_Logger = Logger::get_instance();
}

void Connection::createConnection(const char* PORT) {
	WSADATA wsaData;
	ADDRINFO hints;
	ADDRINFO* addrResult = NULL;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		_Logger->addLog("ERROR", "WSAStartup error", 2);
		WSACleanup();
	}
	else if (getaddrinfo(NULL, PORT, &hints, &addrResult) != 0) {
		_Logger->addLog("ERROR", "getaddrinfo error", 2);
		WSACleanup();
		freeaddrinfo(addrResult);
	}
	else if ((listenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol)) == INVALID_SOCKET) {
		_Logger->addLog("ERROR", "socket error", 2);
		WSACleanup();
		freeaddrinfo(addrResult);
		closesocket(listenSocket);
	}
	else if (bind(listenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen) == SOCKET_ERROR) {
		_Logger->addLog("ERROR", "bind error", 2);
		WSACleanup();
		freeaddrinfo(addrResult);
		closesocket(listenSocket);
	}
	else if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		_Logger->addLog("ERROR", "listen error", 2);
		WSACleanup();
		freeaddrinfo(addrResult);
		closesocket(listenSocket);
	}
	else {
		std::string port = PORT;
		_Logger->addLog("INFO", "Server is started on port: " + port, 2);
	}

	
	FD_ZERO(&masterSet);
	FD_SET(listenSocket, &masterSet);
}

fd_set Connection::getMasterSet(){
	return masterSet;
}

SOCKET Connection::getListenSocket() {
	return listenSocket;
}
