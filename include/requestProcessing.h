#ifndef REQUESTPROCESSING_H
#define REQUESTPROCESSING_H

#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <functional>

#include "../include/logger.h"
#include "../include/config.h"
#include "../include/ClientRequestStruct.pb.h"

//class AutoUpdate;
class SendResponse;

class RequestProcessing {
private:
	Config* _Config;
	Logger* _Logger;

	std::shared_ptr<SendResponse> _SendResponse;

public:
	RequestProcessing();
	~RequestProcessing() = default;
		
	int requestDistribution(std::string requestStr, boost::asio::ip::tcp::socket& socket);
	void filesHashes(ClientRequestStruct::Request acceptedRequest, boost::asio::ip::tcp::socket& socket);
	void file(ClientRequestStruct::Request acceptedRequest, boost::asio::ip::tcp::socket& socket);
};

#endif 