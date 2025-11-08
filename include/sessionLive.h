#ifndef SESSIONLIVE_H
#define SESSIONLIVE_H

#include "logger.h"
#include "config.h"
#include "sessionManager.h"

#include <memory>
#include <chrono>

class ÑlientShutdown;
//class SendResponse;

class SessionLive {
private:
	Config* _Config;
	Logger* _Logger;
	SessionManager* _SessionManager;

	//std::shared_ptr<SendResponse> _SendResponse;
	std::shared_ptr<ÑlientShutdown> _ClientShutdown;

	const std::string MODULE_NAME_ = "(SessionLive)";
public:
	SessionLive();
	~SessionLive() = default;

	void checkActivity();
	// void checkClientSessionStatus(); // check client is live now or no
};

#endif 