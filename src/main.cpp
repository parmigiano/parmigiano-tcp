#include "main.h"

#include "session/tcpServer.h"
#include "scheduler.h"
#include "session/usersQueue.h"

//#include <iostream> // fu ubrat'
//#include <map>
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
	_Config = Config::get_instance();
	_Config->initialize();

	_Logger = Logger::get_instance();
	_Logger->initialize();

	_Database = Database::get_instance();

	_UsersQueue = std::make_shared<UsersQueue>();
	_Scheduler = std::make_shared<Scheduler>();
	_TcpServer = std::make_shared<TcpServer>(io_context, std::stoi(_Config->configuration_vars_["server_port"]));
}

//static void lolkek() {
//	std::cout << 12123 << std::endl;
//}

int AppControl::startApp() {
	std::thread(&UsersQueue::queueHandler, _UsersQueue.get()).detach();

	_Scheduler->start();

	_Database->initialize(
		" password=" + _Config->configuration_vars_["DB_PASSWORD"] +
		" dbname=" + _Config->configuration_vars_["DB_NAME"] +
		" port=" + _Config->configuration_vars_["DB_PORT"] +
		" host=" + _Config->configuration_vars_["DB_ADDRESS"] +
		" user=" + _Config->configuration_vars_["DB_USER"]
	);

	io_context.run();

	return 0;
}