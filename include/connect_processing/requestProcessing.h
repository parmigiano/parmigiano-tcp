#pragma once

#include "logger.h"
#include "config.h"
#include "connect_processing/clientContextStruct.h"
#include "ClientRequestStruct.pb.h"
#include "session/sessionManager.h"

#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <functional>
#include <unordered_map>

class SendResponse;
class UserStatusService;
class ClientInfoCheck;

class UserDeleteMessage;
class UserEditMessage;
class UserPinMessage;
class UserReadMessage;
class UserSendMessage;
class UserTyping;

class RequestProcessing {
private:
	Config* _Config;
	Logger* _Logger;
	SessionManager* _SessionManager;

	std::shared_ptr<SendResponse> _SendResponse;
	std::shared_ptr<UserStatusService> _UserStatusService;
	std::shared_ptr<ClientInfoCheck> _ClientInfoCheck;

	std::shared_ptr<UserDeleteMessage> _UserDeleteMessage;
	std::shared_ptr<UserEditMessage> _UserEditMessage;
	std::shared_ptr<UserPinMessage> _UserPinMessage;
	std::shared_ptr<UserReadMessage> _UserReadMessage;
	std::shared_ptr<UserSendMessage> _UserSendMessage;
	std::shared_ptr<UserTyping> _UserTyping;

    std::unordered_map<ClientRequestStruct::RequestInfo_requestTypes, std::function<void(ClientContext& ctx)>> distribution_map_;
    std::map<ClientRequestStruct::RequestInfo_requestTypes, std::string> request_types_name_map_;

	void initDistMap();
	void initNamesMap();

	const std::string MODULE_NAME_ = "(RequestProcessing)";
public:
	RequestProcessing();
	~RequestProcessing() = default;

	void requestDistribution(std::string request_str, boost::asio::ip::tcp::socket& client_socket);

	void handleUserOnlineStatus(ClientContext& context);
	void handleWelcomePacket(ClientContext& context);
};