#include "../include/acceptingResponses.h"
#include "../include/connection.h"
#include "../include/autoUpdate.h"
//#include "../include/serialization.h"
//#include "../include/responseProcessing.h"

//#include "../json-develop/single_include/nlohmann/json.hpp"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <filesystem>
//#include <queue>

//using json = nlohmann::json;

std::string AcceptingResponses::acceptingServerResponseJson(){
	Connection _connection;
	//Serialization _serialization;
	//AutoUpdate _autoUpdate;

	SOCKET connectSocket = _connection.getCurrentConnectSocket();
	char recvBuffer[1024];

	while (true) {
		memset(recvBuffer, 0, sizeof(recvBuffer));
		if (recv(connectSocket, recvBuffer, sizeof(recvBuffer), 0) > 0) {
			//json serverResponse = json::parse(recvBuffer);

			//std::cout << serverResponse.dump() << std::endl;

			return recvBuffer;

			/*ResponseProcessing _ResponseProcessing;
			_ResponseProcessing.responseDistribution(recvBuffer);*/

			//ofstream file("received_file.exe", ios::app | ios::binary); // write received bytes too temp
			//file.write(recvBuffer, sizeof(recvBuffer)); 
			//std::map <std::string, std::string> acceptedData = _serialization.responseStringToMap(_serialization.deserialize(recvBuffer));

			/*for (auto& a : acceptedData) {
				std::cout << a.first << ":" << a.second << std::endl;
			}*/
			/*if (acceptedData["responseType"] == "update") {
				acceptedData.erase("responseType");
				_autoUpdate.filesComparison(acceptedData);
				break;
			}*/
			//file.close();
		}
	}
}

int AcceptingResponses::acceptingFiles(std::string filePath){
	Connection _connection;
	//Serialization _serialization;
	//AutoUpdate _autoUpdate;

	SOCKET connectSocket = _connection.getCurrentConnectSocket();
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
		//std::cout << recvBuffer << std::endl;

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