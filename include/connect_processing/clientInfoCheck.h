#pragma once

//#include "sessionManager.h"
#include "logger.h"
#include "connect_processing/disconnectTypeEnum.h"
#include "ClientRequestStruct.pb.h"

#include <string>

class ClientShutdown;
class Session;

class ClientInfoCheck {
private:
	Logger* _Logger;
	//SessionManager* _SessionManager;

	std::shared_ptr<ClientShutdown> _ClientShutdown;

	bool checkUID(uint64_t& UID, Session* session);

	const std::string MODULE_NAME_ = "(ClientInfoCheck)";
public:
	ClientInfoCheck();
	~ClientInfoCheck() = default;

	bool checkInfoFullness(ClientRequestStruct::Request& accepted_request, Session* session);
};