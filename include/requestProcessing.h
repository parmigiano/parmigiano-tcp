#ifndef REQUESTPROCESSING_H
#define REQUESTPROCESSING_H

#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <memory>

#include "../include/logger.h"
//#include "../include/sendResponse.h"
//#include "../include/autoUpdate.h"

//class AutoUpdate;
class SendResponse;

class RequestProcessing {
private:
	Logger* _Logger;

	/*AutoUpdate* _AutoUpdate = nullptr;
	SendResponse* _SendResponse = nullptr;*/

	//std::shared_ptr<AutoUpdate> _AutoUpdate;
	//std::unique_ptr<AutoUpdate> _AutoUpdate;
	std::shared_ptr<SendResponse> _SendResponse;

public:
	RequestProcessing();
	~RequestProcessing() = default;
		
	int requestDistribution(std::string request, SOCKET clientCoket);
};

//extern requestProcessing _requestProcessing;

#endif 