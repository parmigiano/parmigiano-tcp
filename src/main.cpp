#include "../include/main.h"
#include "../include/config.h"
#include "../include/connect.h"
#include "../include/connectionHandler.h"
#include "../include/autoUpdate.h"
#include "../include/logger.h"

#include <iostream>
#include <map>
#include <thread>

int main() {
	setlocale(LC_ALL, "ru");
	appControl _appControl;

	_appControl.startApp();

	system("pause");

	return 0;
}

void appControl::startApp() {
	//Config _Config;
	Connection _Connection;
	ConnectionHandler _ConnectionHandler;
	AutoUpdate _AutoUpdate;

	//_AutoUpdate;

	//_AutoUpdate.getRequiredFileHashes
	
	_Config.parseConfig();
	_Logger.initializeLogger();

	_Connection.createConnection(std::to_string(_Config.serverPort).c_str());

	//_Logger.addLog("INFO", "server is started");

	_AutoUpdate.collectFilesInfo();

	thread(&ConnectionHandler::queueHandler, &_ConnectionHandler).detach();

	_ConnectionHandler.incomingConnections();
}