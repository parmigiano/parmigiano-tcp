#ifndef REQUESTPROCESSING_H
#define REQUESTPROCESSING_H

#include "logger.h"
#include "config.h"
#include "ClientRequestStruct.pb.h"
#include "sessionManager.h"

#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <functional>
#include <unordered_map>
//#include <pqxx/pqxx>

class SendResponse;
class UserStatusNotify;
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
	std::shared_ptr<UserStatusNotify> _UserStatusNotify;
	std::shared_ptr<ClientInfoCheck> _ClientInfoCheck;

	const std::string MODULE_NAME_ = "(RequestProcessing)";
public:
	RequestProcessing();
	~RequestProcessing() = default;
		
	void requestDistribution(std::string request_str, boost::asio::ip::tcp::socket& client_sock);

	void sendMessage(ClientContext&);
	void readMessage(ClientContext&);
	void editMessage(ClientContext&);
	void pinMessage(ClientContext&);
	void deleteMessage(ClientContext&);

	void userTyping(ClientContext&);
	void userOnlineStatus(ClientContext&);
	void getUnreadMessageCount(ClientContext&);
};

#endif 