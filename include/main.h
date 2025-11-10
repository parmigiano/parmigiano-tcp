#pragma once

#include "logger.h"
#include "config.h"

#include "database/database.h" // temp

#include <memory>
#include <boost/asio.hpp>

class TcpServer;
class UsersQueue;
class Scheduler;
//class Task;

class AppControl {
private:
	Logger* _Logger;
	Config* _Config;
	Database* _Database;

	std::shared_ptr<TcpServer> _TcpServer;
	std::shared_ptr<UsersQueue> _UsersQueue;
	std::shared_ptr<Scheduler> _Scheduler;
	//std::shared_ptr<Task> _Task;

	boost::asio::io_context io_context;
public:
	AppControl();
	~AppControl() = default;

	int startApp();
};