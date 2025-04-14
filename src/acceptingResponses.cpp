#include "../include/acceptingResponses.h"
#include "../include/connection.h"
#include "../include/autoUpdate.h"
#include "../include/serialization.h"
#include "../include/responseProcessing.h"

#include "../json-develop/single_include/nlohmann/json.hpp"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <fstream> // temp
#include <filesystem>

using json = nlohmann::json;

std::string AcceptingResponses::acceptingServerResponseJson(){
	Connection _connection;
	Serialization _serialization;
	AutoUpdate _autoUpdate;

	SOCKET connectSocket = _connection.getCurrentConnectSocket();
	char recvBuffer[1024];

	while (true) {
		memset(recvBuffer, 0, sizeof(recvBuffer));
		if (recv(connectSocket, recvBuffer, sizeof(recvBuffer), 0) > 0) {
			json serverResponse = json::parse(recvBuffer);

			std::cout << serverResponse.dump() << std::endl;

			if (serverResponse["responseInfo"]["responseType"] == "actualClientFilesHashes") {
				return recvBuffer;
			}

			ResponseProcessing _ResponseProcessing;
			_ResponseProcessing.responseDistribution(recvBuffer);

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

std::string AcceptingResponses::acceptingFiles(){
	Connection _connection;
	Serialization _serialization;
	AutoUpdate _autoUpdate;

	SOCKET connectSocket = _connection.getCurrentConnectSocket();
	char recvBuffer[4096];

	while (true) {
		memset(recvBuffer, 0, sizeof(recvBuffer));
		if (recv(connectSocket, recvBuffer, sizeof(recvBuffer), 0) > 0) {

			return recvBuffer;
			//ofstream file("received_file.exe", ios::app | ios::binary); // write received bytes too temp
			//file.write(recvBuffer, sizeof(recvBuffer));

			//_autoUpdate.recordingFiles(_serialization.responseStringToMap());
			
			//file.close();
		}
		else {
			break;
		}
	}
}
