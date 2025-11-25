#pragma once

#define ASIO_STANDALONE

#include "logger.h"
#include "config.h"
#include "session/usersQueue.h"
#include "session/sessionManager.h"

#include <boost/asio.hpp>

//class UsersQueue;
class UserStatusService;

class Session : public std::enable_shared_from_this<Session> {
private:
	Logger* _Logger;
	Config* _Config;
	UsersQueue* _UsersQueue;
	SessionManager* _SessionManager;

	//std::shared_ptr<UsersQueue> _UsersQueue;
	std::shared_ptr<UserStatusService> _UserStatusService;

	void handleDisconnect(const boost::system::error_code& error);
	void readHeader(const boost::system::error_code& error, size_t bytes);
	void readBody(const boost::system::error_code& error, size_t bytes);

	boost::asio::ip::tcp::socket client_socket_;
	/*enum { max_length = 1024 };
	char data_[max_length];*/

	uint32_t msg_length_; 
	std::vector<char> msg_data_;

	const std::string MODULE_NAME_ = "(Session)";
public:
	Session(boost::asio::io_context& io_context);
	~Session() = default;

	//typedef std::shared_ptr<Session> pointer;

	std::shared_ptr<Session> create(boost::asio::io_context& io_context);
	boost::asio::ip::tcp::socket& socket();
	void handleWrite(const boost::system::error_code& error, size_t bytes);
	void start();
};