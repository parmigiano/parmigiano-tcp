#ifndef REQUESTPROCESSING_H
#define REQUESTPROCESSING_H

#include "../include/logger.h"
#include "../include/config.h"
#include "ClientRequestStruct.pb.h"

#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <functional>

class SendResponse;
class SessionManager;

class RequestProcessing {
private:
	Config* _Config;
	Logger* _Logger;

	std::shared_ptr<SendResponse> _SendResponse;
	std::shared_ptr<SessionManager> _SessionManager;

public:
	RequestProcessing();
	~RequestProcessing() = default;
		
	int requestDistribution(std::string requestStr, boost::asio::ip::tcp::socket& socket);
};

#endif 