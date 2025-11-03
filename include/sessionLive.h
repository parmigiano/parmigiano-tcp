#ifndef SESSIONLIVE_H
#define SESSIONLIVE_H

#include "../include/logger.h"
#include "../include/config.h"
#include "../include/sessionManager.h"

#include <memory>
#include <chrono>

class ForcedClientShutdown;
//class SendResponse;

class SessionLive {
private:
	Config* _Config;
	Logger* _Logger;
	SessionManager* _SessionManager;

	//std::shared_ptr<SendResponse> _SendResponse;
	std::shared_ptr<ForcedClientShutdown> _ForcedClientShutdown;
public:
	SessionLive();
	~SessionLive() = default;

	void checkActivity();
	// void checkClientSessionStatus(); // check client is live now or no
};

#endif 