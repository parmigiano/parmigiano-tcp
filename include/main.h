#ifndef MAIN_H
#define MAIN_H

#include <memory>

#include "../include/logger.h"
#include "../include/config.h"

//#include "../include/autoUpdate.h"
//#include "../include/connect.h"
//#include "../include/connectionHandler.h"

class AutoUpdate;
class Connection;
class ConnectionHandler;

class AppControl {
private:
	Logger* _Logger;
	Config* _Config;

	/*AutoUpdate* _AutoUpdate = nullptr;
	Connection* _Connection = nullptr;
	ConnectionHandler* _ConnectionHandler = nullptr;*/

	std::shared_ptr<AutoUpdate> _AutoUpdate;
	std::shared_ptr<Connection> _Connection;
	std::shared_ptr<ConnectionHandler> _ConnectionHandler;
public:
	AppControl();
	~AppControl() = default;

	int startApp();
};

//extern appControl _appControl;

#endif 