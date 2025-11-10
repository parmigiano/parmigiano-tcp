#pragma once

#include "logger.h"
#include "config.h"
#include "ClientRequestStruct.pb.h"
#include "session/sessionManager.h"

#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <functional>
#include <unordered_map>
//#include <pqxx/pqxx>

class SendResponse;
class UserStatusService;
class ClientInfoCheck;
// class SessionManager;

struct ClientContext {
	ClientRequestStruct::Request request;
	boost::asio::ip::tcp::socket& socket; 
};

class RequestProcessing {
private:
	Config* _Config;
	Logger* _Logger;
	SessionManager* _SessionManager;

	std::shared_ptr<SendResponse> _SendResponse;
	std::shared_ptr<UserStatusService> _UserStatusService;
	std::shared_ptr<ClientInfoCheck> _ClientInfoCheck;

	//std::unordered_map<ClientRequestStruct::RequestInfo_requestTypes, std::function<void(ClientContext& ctx)>> distribution_map_;

	const std::string MODULE_NAME_ = "(RequestProcessing)";
public:
	RequestProcessing();
	~RequestProcessing() = default;
		
	void requestDistribution(std::string request_str, boost::asio::ip::tcp::socket& client_socket);

	void sendMessage(ClientContext&);
	void readMessage(ClientContext&);
	void editMessage(ClientContext&);
	void pinMessage(ClientContext&);
	void deleteMessage(ClientContext&);

	void userTyping(ClientContext&);
	void userOnlineStatus(ClientContext&);
	void getUnreadMessageCount(ClientContext&);
};