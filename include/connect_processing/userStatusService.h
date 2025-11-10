#pragma once

#include "session/sessionManager.h"
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

	uint64_t getUID(boost::asio::ip::tcp::socket& client_sock);

	const std::string MODULE_NAME_ = "(UserStatusService)";
public:
	UserStatusService();
	~UserStatusService() = default;

	void notifyOffline(boost::asio::ip::tcp::socket& client_sock);
	void notifyOffline(uint64_t& UID);

	void notifyOnline(boost::asio::ip::tcp::socket& client_sock);
	void notifyOnline(uint64_t& UID);
};