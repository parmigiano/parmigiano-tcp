#ifndef SENDRESPONSE_H
#define SENDRESPONSE_H

#include "../include/logger.h"
#include "../include/config.h"
#include "ClientRequestStruct.pb.h"
#include "ResponseStruct.pb.h"

#include <string>
#include <fstream>
#include <memory>
#include <boost/asio.hpp>

class Session;

class SendResponse {
public:
	enum disconnectType { warn, error, tempBan, inactive, unknown };
private:
	Logger* _Logger;
	Config* _Config;

	std::shared_ptr<ResponseStruct::Response> _Response;
	std::shared_ptr<Session> _Session;
public:
	SendResponse();
	~SendResponse() = default;

	void setReponseType(std::string responseType);
	void setDisonnectType(disconnectType);
	void setDisconnectDescription(std::string description);
	void setDisconnectCode(short int code);
	void clearResponse();
	std::ifstream openFile(std::string filepath, unsigned int fileSize);

	int sendResponse(boost::asio::ip::tcp::socket& socket);
	int sendFile(std::string filePath, boost::asio::ip::tcp::socket& socket);
};

#endif 