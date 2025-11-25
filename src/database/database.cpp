#include "database/database.h"

std::shared_ptr<pqxx::connection> _Connection;

Database::Database() {
	_Logger = Logger::get_instance();
	_Config = Config::get_instance();
}

void Database::initialize(const std::string& conn_str) {
	connection_string_ = conn_str;
	createConnection();
}

void Database::createConnection() {
	try {
		_Connection = std::shared_ptr<pqxx::connection>(
			new pqxx::connection(connection_string_),
			[](pqxx::connection* connection) {
				if (connection && connection->is_open()) {
					connection->close();
				}
				delete connection;
			}
		);

		if (_Connection && _Connection->is_open()) {
			_Logger->addServerLog(_Logger->info, MODULE_NAME_ + " Successful connect", 2);
		}
		else {
			throw std::runtime_error("couldnt connect to database");
		}
	}
	catch (const std::exception& error) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
	}
	catch (...) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
	}
}

void Database::diconnect() {
	try {
		_Connection->close();
	}
	catch (const std::exception& error) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
	}
	catch (...) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
	}
}

void Database::reconnect() {
	diconnect();
	createConnection();
}

std::shared_ptr<pqxx::connection> Database::getConnection() {
	return _Connection;
}
