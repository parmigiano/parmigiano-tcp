#pragma once

#include "logger.h"
#include "config.h"
#include "session/sessionManager.h"
#include <connect_processing/disconnectTypeEnum.h>

#include <memory>
#include <chrono>

class ClientShutdown;
//class SendResponse;

class SessionLive {
private:
	Config* _Config;
	Logger* _Logger;
	SessionManager* _SessionManager;

	//std::shared_ptr<SendResponse> _SendResponse;
	std::shared_ptr<ClientShutdown> _ClientShutdown;

	const std::string MODULE_NAME_ = "(SessionLive)";
public:
	SessionLive();
	~SessionLive() = default;

	void checkActivity();
	// void checkClientSessionStatus(); // check client is live now or no
};