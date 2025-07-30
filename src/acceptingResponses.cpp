#include "../include/acceptingResponses.h"
#include "../include/connection.h"
//#include "../include/autoUpdate.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <filesystem>

AcceptingResponses::AcceptingResponses(){
	_Connection = std::make_shared<Connection>();
}

std::string AcceptingResponses::acceptingServerResponseJson(){
	//Connection _connection;

	SOCKET connectSocket = _Connection->getCurrentConnectSocket();
	char recvBuffer[1024];

	while (true) {
		memset(recvBuffer, 0, sizeof(recvBuffer));
		if (recv(connectSocket, recvBuffer, sizeof(recvBuffer), 0) > 0) {

			return recvBuffer;
		}
	}
}

int AcceptingResponses::acceptingFiles(std::string filePath){
	//Connection _connection;

	SOCKET connectSocket = _Connection->getCurrentConnectSocket();
	char recvBuffer[1024];
	DWORD bytesRead;
	DWORD writtenBytes;

	HANDLE hFile = CreateFileA(
		filePath.c_str(),
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hFile == INVALID_HANDLE_VALUE) {
		std::cerr << "Failed to open file handle with error: " << GetLastError() << std::endl;
		return 1;
	}

	while ((bytesRead = recv(connectSocket, recvBuffer, sizeof(recvBuffer), 0)) > 0) {

		if (strcmp(recvBuffer, "end") == 0) {
			break;
		}

		if (!WriteFile(hFile, recvBuffer, bytesRead, &writtenBytes, NULL)) {
			std::cerr << "Accepting file failed with error: " << GetLastError() << std::endl;
			return 1;
		}

		memset(recvBuffer, 0, sizeof(recvBuffer));
	}

	CloseHandle(hFile);

	return 0;
}