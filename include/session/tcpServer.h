#pragma once

#define ASIO_STANDALONE

#include "logger.h"
#include "config.h"

#include <boost/asio.hpp>
#include <memory>

class Session;

class TcpServer {
private:
	Logger* _Logger;
	Config* _Config;

	//std::shared_ptr<Session> Session_;

	boost::asio::io_context& io_context_;
	boost::asio::ip::tcp::acceptor acceptor_;

	const std::string MODULE_NAME_ = "(TcpServer)";
public:
	TcpServer(boost::asio::io_context& io_context, int port);
	~TcpServer() = default;

	void start_accept();
	void handle_accept(std::shared_ptr<Session> session, const boost::system::error_code& error);
};