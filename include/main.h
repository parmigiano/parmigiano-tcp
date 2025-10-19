#ifndef MAIN_H
#define MAIN_H

#include <memory>
#include <boost/asio.hpp>

#include "../include/logger.h"
#include "../include/config.h"

class AutoUpdate;
//class Connection;
class TcpServer;
class UsersQueue;

class AppControl {
private:
	Logger* _Logger;
	Config* _Config;

	std::shared_ptr<AutoUpdate> _AutoUpdate;
	//std::shared_ptr<Connection> _Connection;
	std::shared_ptr<TcpServer> _TcpServer;
	std::shared_ptr<UsersQueue> _UsersQueue;

	boost::asio::io_context io_context;
	uint8_t configLoadResult = 0;
public:
	AppControl();
	~AppControl() = default;

	int startApp();
};

#endif 