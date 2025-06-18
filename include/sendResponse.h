#ifndef SENDRESPONSE_H
#define SENDRESPONSE_H

#include "../json-develop/single_include/nlohmann/json.hpp"
#include "../include/logger.h"
#include "../include/config.h"
//#include "../include/connectionHandler.h"

#include <string>
#include <iostream>
#include <map>
#include <WinSock2.h>
#include <memory>

//class ConnectionHandler;

using json = nlohmann::json;

class SendResponse {
private:
	Logger* _Logger;
	Config* _Config;

	//ConnectionHandler* _ConnectionHandler = nullptr;

	//std::shared_ptr<ConnectionHandler> _ConnectionHandler;

public:
	SendResponse();
	~SendResponse() = default;

	int sendJSON(json j, std::string responseType, SOCKET clientSocket);
	int sendFile(std::string filePath, std::string fileName, SOCKET clientSocket);
};

//extern SendResponse _SendResponse;

#endif 