#pragma once

#include "session/sessionManager.h"
#include "connect_processing/responseTypeEnum.h"
#include "logger.h"

#include <memory>
#include <string>
#include <vector>
#include <boost/asio.hpp>

class SendResponse;
class ChatMembersTable;
class UserActivesTable;

class UserStatusService {
private:
	Logger* _Logger;
	SessionManager* _SessionManager;

	std::shared_ptr<SendResponse> _SendResponse;
	std::shared_ptr<ChatMembersTable> _ChatMembers;
	std::shared_ptr<UserActivesTable> _UserActives;

	const std::string MODULE_NAME_ = "(UserStatusService)";
public:
	UserStatusService();
	~UserStatusService() = default;

	//void notifyOffline(Session& session);
	void notifyOffline(Session& session);

	//void notifyOnline(Session& session);
	void notifyOnline(Session& session);
};