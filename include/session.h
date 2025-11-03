#ifndef SESSION_H
#define SESSION_H

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00 // force warn for win system
#endif

#define ASIO_STANDALONE

#include "../include/logger.h"
#include "../include/config.h"

#include <boost/asio.hpp>

class UsersQueue;

class Session : public std::enable_shared_from_this<Session> {
private:
	Logger* _Logger;
	Config* _Config;

	std::shared_ptr<UsersQueue> _UsersQueue;

	void handle_disconnect(const boost::system::error_code& error);
	void handle_read(const boost::system::error_code& error, size_t bytes);

	boost::asio::ip::tcp::socket socket_;
	enum { max_length = 1024 };
	char data_[max_length];
public:
	Session(boost::asio::io_context& io_context);
	~Session() = default;

	typedef std::shared_ptr<Session> pointer;

	pointer create(boost::asio::io_context& io_context);
	boost::asio::ip::tcp::socket& socket();
	void handle_write(const boost::system::error_code& error, size_t bytes);
	void start();
};

#endif 