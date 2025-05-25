#include "../include/connect.h"

#include <iostream>

SOCKET listenSocket = INVALID_SOCKET;
fd_set masterSet;

void Connection::createConnection(const char* PORT) {
	WSADATA wsaData;
	ADDRINFO hints;
	ADDRINFO* addrResult = NULL;
	//SOCKET clientSocket = INVALID_SOCKET;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "(!) WSAStartup error\n";
		WSACleanup();
	}
	else if (getaddrinfo(NULL, PORT, &hints, &addrResult) != 0) {
		std::cerr << "(!) getaddrinfo error\n";
		WSACleanup();
		freeaddrinfo(addrResult);
	}
	else if ((listenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol)) == INVALID_SOCKET) {
		std::cerr << "(!) socket error\n";
		WSACleanup();
		freeaddrinfo(addrResult);
		closesocket(listenSocket);
	}
	else if (bind(listenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen) == SOCKET_ERROR) {
		std::cerr << "(!) bind error\n";
		WSACleanup();
		freeaddrinfo(addrResult);
		closesocket(listenSocket);
	}
	else if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cerr << "(!) listen error\n";
		WSACleanup();
		freeaddrinfo(addrResult);
		closesocket(listenSocket);
	}
	else {
		//CH.incomingConnections();
		std::cout << "Server is started on port: " << PORT << std::endl;
	}

	
	FD_ZERO(&masterSet);
	FD_SET(listenSocket, &masterSet);
}

fd_set Connection::getMasterSet(){
	return masterSet;
}

SOCKET Connection::getListenSocket() {
	if (listenSocket != INVALID_SOCKET) {
		return listenSocket;
	}
}


