#include "../include/connection.h"

#include <iostream>

SOCKET connectSocket = INVALID_SOCKET;
//const char* IPADDRESS = "localhost";

void Connection::createConnect(const char* PORT, const char* IPADDRESS){
	WSADATA wsaData;
	ADDRINFO hints;
	ADDRINFO* addrResult = NULL;
	//SOCKET clientSocket = INVALID_SOCKET;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == 1) {
		std::cout << "(!) WSAStartup error\n";
		WSACleanup();
		tryConnectAgain(PORT, IPADDRESS);
	}
	else if (getaddrinfo(IPADDRESS, PORT, &hints, &addrResult) == 1) {
		std::cout << "(!) getaddrinfo error\n";
		WSACleanup();
		freeaddrinfo(addrResult);
		tryConnectAgain(PORT, IPADDRESS);
	}
	else if ((connectSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol)) == INVALID_SOCKET) {
		std::cout << "(!) socket error\n";
		WSACleanup();
		freeaddrinfo(addrResult);
		closesocket(connectSocket);
		tryConnectAgain(PORT, IPADDRESS);
	}
	else if (connect(connectSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen) == SOCKET_ERROR) {
		std::cout << "(!) connect error\n";
		WSACleanup();
		freeaddrinfo(addrResult);
		closesocket(connectSocket);
		tryConnectAgain(PORT, IPADDRESS);
	}
	else {
		std::cout << "(!) successful connect :)" << std::endl;
		/*while (true) {
		}*/
	}
}

void Connection::tryConnectAgain(const char* PORT, const char* IPADDRESS){
	Sleep(5000);
	createConnect(PORT, IPADDRESS);
}

int Connection::connectToServer(const char* PORT, const char* IPADDRESS){
	try {
		createConnect(PORT, IPADDRESS);
		return 0;
	} catch (const std::exception& ex) {
		std::cout << "Error with connect to server! " << ex.what() << std::endl;
		return 1;
	}
}

int Connection::disconnectFromCurrentServer(){
	if (connectSocket == INVALID_SOCKET) {
		std::cout << "Socket currently closed or invalid" << std::endl;
		return 1;
	}

	if (shutdown(connectSocket, SD_BOTH) == SOCKET_ERROR) {
		std::cout << "Error with close connect: " << WSAGetLastError() << std::endl;
		closesocket(connectSocket);
		connectSocket = INVALID_SOCKET;
		return 1;
	}

	if (closesocket(connectSocket) == SOCKET_ERROR) {
		std::cout << "Error with close socket: " << WSAGetLastError() << std::endl;
		connectSocket = INVALID_SOCKET;
		return 1;
	}

	connectSocket = INVALID_SOCKET;

	WSACleanup();
	return 0;
}

SOCKET Connection::getCurrentConnectSocket(){
	return connectSocket;
}
