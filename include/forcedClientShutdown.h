#ifndef FORCEDCLIENTSHUTDOWN_H
#define FORCEDCLIENTSHUTDOWN_H

#include "../include/logger.h"
#include "../include/config.h"
#include "../include/sessionManager.h"

#include <memory>
#include <string>
#include <boost/asio.hpp>

class SendResponse;

class ForcedClientShutdown {
public:
	enum disconnectType { warn, error, tempBan, inactive };
private:
	Config* _Config;
	Logger* _Logger;
	SessionManager* _SessionManager;

	std::shared_ptr<SendResponse> _SendResponse;

	void disconnect(std::string& UID);
	void notify(std::string& UID, std::string description, short int code, disconnectType&);
public:
	ForcedClientShutdown();
	~ForcedClientShutdown() = default;

	void disconnectClientByReason(std::string& UID, std::string description, short int code, disconnectType);
};

#endif 