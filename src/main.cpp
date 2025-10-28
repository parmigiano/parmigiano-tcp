#include "../include/main.h"

#include "../include/autoUpdate.h"
#include "../include/tcpServer.h"
#include "../include/usersQueue.h"

#include <iostream>
#include <map>
#include <thread>

int main() {
	setlocale(LC_ALL, "ru");
	AppControl* _appControl = new AppControl();

	_appControl->startApp();

	delete _appControl;
	system("pause");

	return 0;
}

AppControl::AppControl(){
	_Logger = Logger::get_instance();
	_Logger->initializeLogger();

	_Config = Config::get_instance();
	_Config->parseConfig();

	_AutoUpdate = std::make_shared<AutoUpdate>();
	_UsersQueue = std::make_shared<UsersQueue>();
	_TcpServer = std::make_shared<TcpServer>(io_context, std::stoi(_Config->configurationVars["serverPort"]));
}

int AppControl::startApp() {
	_AutoUpdate->collectFilesInfo();

	std::thread(&UsersQueue::queueHandler, _UsersQueue.get()).detach();

	io_context.run();

	return 0;
}