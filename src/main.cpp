#include "../include/main.h"

#include "../include/tcpServer.h"
#include "../include/usersQueue.h"

#include <iostream>
#include <map>
#include <thread>

int main() {
	setlocale(LC_ALL, "ru");

	std::shared_ptr<AppControl> _appControl;
	_appControl = std::make_shared<AppControl>();

	_appControl->startApp();

	system("pause");

	return 0;
}

AppControl::AppControl(){
	_Logger = Logger::get_instance();
	_Logger->initializeLogger();

	_Config = Config::get_instance();
	_Config->parseConfig();

	_UsersQueue = std::make_shared<UsersQueue>();
	_TcpServer = std::make_shared<TcpServer>(io_context, std::stoi(_Config->configurationVars["serverPort"]));
}

int AppControl::startApp() {
	std::thread(&UsersQueue::queueHandler, _UsersQueue.get()).detach();

	io_context.run();

	return 0;
}