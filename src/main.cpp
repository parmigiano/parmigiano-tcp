#include "../include/main.h"

#include "../include/autoUpdate.h"
#include "../include/connect.h"
#include "../include/connectionHandler.h"

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
	_Config = Config::get_instance();

	_AutoUpdate = std::make_shared<AutoUpdate>();
	_Connection = std::make_shared<Connection>();
	_ConnectionHandler = std::make_shared<ConnectionHandler>();
}

int AppControl::startApp() {
	short int configLoadResult = _Config->parseConfig();
	_Logger->initializeLogger();

	if (configLoadResult > 0) {
		_Logger->addLog("WARN", "Probmlem with config file: \"config.txt\"", 2);
		_Logger->addLog("WARN", "Configure it for succesfull start server", 2);

		return 1;
	}

	_Logger->addLog("INFO", "Starting server...", 2);

	_Connection->createConnection(std::to_string(_Config->serverPort).c_str());

	_AutoUpdate->collectFilesInfo();
	_AutoUpdate->initializeLatestBuildInfo();

	std::thread(&ConnectionHandler::queueHandler, _ConnectionHandler.get()).detach();

	_ConnectionHandler->incomingConnections();

	delete _Config;

	return 0;
}