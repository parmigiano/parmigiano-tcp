#ifndef CLIENTINFOCHECK_H
#define CLIENTINFOCHECK_H

//#include "sessionManager.h"
#include "logger.h"
#include "ClientRequestStruct.pb.h"

#include <string>

class СlientShutdown;

class ClientInfoCheck {
private:
	Logger* _Logger;
	//SessionManager* _SessionManager;s

	std::shared_ptr<СlientShutdown> _ClientShutdown;

	bool checkUID(uint64_t& UID, boost::asio::ip::tcp::socket& client_socket);

	const std::string MODULE_NAME_ = "(ClientInfoCheck)";
public:
	ClientInfoCheck();
	~ClientInfoCheck() = default;

	bool checkInfoFullness(ClientRequestStruct::Request& accepted_request, boost::asio::ip::tcp::socket& client_socket);
};

#endif
