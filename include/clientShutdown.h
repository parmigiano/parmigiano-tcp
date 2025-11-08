#ifndef FORCEDCLIENTSHUTDOWN_H
#define FORCEDCLIENTSHUTDOWN_H

#include "logger.h"
#include "config.h"
#include "sessionManager.h"

#include <memory>
#include <string>
#include <boost/asio.hpp>

class SendResponse;

class ÑlientShutdown {
public:
	enum disconnectType {
		warn, 
		error, 
		tempBan, 
		inactive,
		littleInfo
	};

private:
	Config* _Config;
	Logger* _Logger;
	SessionManager* _SessionManager;

	std::shared_ptr<SendResponse> _SendResponse;

	//uint64_t getUID(boost::asio::ip::tcp::socket& client_socket);
	void disconnectCommon(boost::asio::ip::tcp::socket& client_socket);
	void disconnect(uint64_t& UID);
	void disconnect(boost::asio::ip::tcp::socket& client_socket);

	void notifyCommon(boost::asio::ip::tcp::socket& client_socket, std::string& description, short int& code, disconnectType& disconnect_type);
	void notify(uint64_t& UID, std::string description, short int code, disconnectType& disconnect_type);
	void notify(boost::asio::ip::tcp::socket& client_socket, std::string description, short int code, disconnectType& disconnect_type);

	const std::string MODULE_NAME_ = "(ClientShutdown)";
public:
	ÑlientShutdown();
	~ÑlientShutdown() = default;

	void disconnectClientByReason(uint64_t& UID, std::string description, short int code, disconnectType disconnect_type);
	void disconnectClientByReason(boost::asio::ip::tcp::socket& client_socket, std::string description, short int code, disconnectType disconnect_type);
	//void disconnectProcessing(boost::asio::ip::tcp::socket& client_socket);
};

#endif 