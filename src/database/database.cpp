#include "database/database.h"

Database* Database::instance_ptr_ = nullptr;
std::mutex Database::instance_mtx_;

Database::Database() {
	_Logger = Logger::get_instance();
	_Config = Config::get_instance();
}

Database* Database::get_instance() {
	std::lock_guard<std::mutex> lock(instance_mtx_);
	if (instance_ptr_ == nullptr) {
		instance_ptr_ = new Database();
	}

	return instance_ptr_;
}

void Database::initialize(const std::string& conn_str) {
	connection_string_ = conn_str;
	createConnection();
}

void Database::createConnection() {
	try {
		conn_ = std::shared_ptr<pqxx::connection>(
			new pqxx::connection(connection_string_),
			[](pqxx::connection* connection) {
				if (connection && connection->is_open()) {
					connection->close();
				}
				delete connection;
			}
		);

		if (conn_ && conn_->is_open()) {
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
		conn_->close();
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
	return conn_;
}
