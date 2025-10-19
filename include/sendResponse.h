#ifndef SENDRESPONSE_H
#define SENDRESPONSE_H

#include "../include/logger.h"
#include "../include/config.h"
#include "../include/ClientRequestStruct.pb.h"
#include "../include/ResponseStruct.pb.h"

#include <string>
#include <memory>
#include <boost/asio.hpp>

class Session;

class SendResponse {
private:
	Logger* _Logger;
	Config* _Config;

	std::shared_ptr<ResponseStruct::Response> _Response;
	std::shared_ptr<Session> _Session;
public:
	SendResponse();
	~SendResponse() = default;

	void setReponseType(std::string responseType);
	void addMainFileName(std::string fileName);
	void addFileInfo(std::string hash, std::string path, std::string name);
	void addDirInfo(std::string dirPath);
	void clearResponse();
	bool checkFileAvaibility(std::string filePath);

	int sendResponse(boost::asio::ip::tcp::socket& socket);
	int sendFile(std::string filePath, boost::asio::ip::tcp::socket& socket);
};

#endif 