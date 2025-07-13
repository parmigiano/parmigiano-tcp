#ifndef SENDRESPONSE_H
#define SENDRESPONSE_H

#include "../include/logger.h"
#include "../include/config.h"
//#include "../include/connectionHandler.h"
#include "../include/ClientRequestStruct.pb.h"

#include <string>
#include <iostream>
#include <map>
#include <WinSock2.h>
#include <memory>

//class ConnectionHandler;

#include "../include/ResponseStruct.pb.h"

class SendResponse {
private:
	Logger* _Logger;
	Config* _Config;

	std::shared_ptr<ResponseStruct::Response> _Response;

	//ConnectionHandler* _ConnectionHandler = nullptr;

	//std::shared_ptr<ConnectionHandler> _ConnectionHandler;

public:
	SendResponse();
	~SendResponse() = default;

	void setReponseType(std::string responseType);
	void addFileInfo(std::string hash, std::string path, std::string name);
	void addDirInfo(std::string dirPath);
	void clearResponse();

	int sendResponse(SOCKET clientSocket);
	int sendFile(std::string filePath, std::string fileName, SOCKET clientSocket);
};

//extern SendResponse _SendResponse;

#endif 