#ifndef MAIN_H
#define MAIN_H

#include <memory>
#include <boost/asio.hpp>

#include "../include/logger.h"
#include "../include/config.h"

class TcpServer;
class UsersQueue;

class AppControl {
private:
	Logger* _Logger;
	Config* _Config;

	std::shared_ptr<TcpServer> _TcpServer;
	std::shared_ptr<UsersQueue> _UsersQueue;

	boost::asio::io_context io_context;
public:
	AppControl();
	~AppControl() = default;

	int startApp();
};

#endif 