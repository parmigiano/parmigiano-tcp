#ifndef TCPSERVER_H
#define TCPSERVER_H

#define ASIO_STANDALONE
#include <boost/asio.hpp>
#include <memory>

#include "../include/logger.h"
#include "../include/config.h"

class Session;

class TcpServer {
private:
	Logger* _Logger;
	Config* _Config;

	std::shared_ptr<Session> _Session;

	boost::asio::io_context& io_context_;
	boost::asio::ip::tcp::acceptor acceptor_;
public:
	TcpServer(boost::asio::io_context& io_context, int port);
	~TcpServer() = default;

	void start_accept();
	void handle_accept(const boost::system::error_code& error);
};

#endif 