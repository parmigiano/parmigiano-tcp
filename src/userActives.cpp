#include "userActives.h"

UserActives::UserActives() {
	_Database = Database::get_instance();
	_Logger = Logger::get_instance();
	_PreparedStatementManager = PreparedStatementManager::get_instance();
}

void UserActives::initStatements() {
    _PreparedStatementManager->registerStatement("getOnlineStatusByUID", "SELECT user_actives FROM online WHERE user_uid = $1");

    _PreparedStatementManager->registerStatement("setOnlineStatusByUID", "UPDATE user_actives SET online = $1 WHERE user_uid = $2");
}

std::string UserActives::getOnlineStatusByUID(uint64_t& UID) {
	try {
		std::shared_ptr<pqxx::connection> conn = _Database->getConnection();
		pqxx::work transaction(*conn);

		pqxx::result txn_result = _PreparedStatementManager->exec(transaction, "getOnlineStatusByUID", UID);

		transaction.commit();

		return txn_result[0]["online"].as<std::string>();
	}
	catch (const std::exception& error) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
		return "";
	}
	catch (...) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
		return "";
	}
}

void UserActives::setOnlineStatusByUID(uint64_t& UID, onlineStatus status_type) {
	try {
		bool status;
		std::shared_ptr<pqxx::connection> conn = _Database->getConnection();
		pqxx::work transaction(*conn);

		switch (status_type) {
		case UserActives::online:
			status = true;
			break;
		case UserActives::offline:
			status = false;
			break;
		default:
			status = false;
			break;
		}

		pqxx::result txn_result = _PreparedStatementManager->exec(transaction, "setOnlineStatusByUID", status, UID);

		transaction.commit();
	}
	catch (const std::exception& error) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
	}
	catch (...) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
	}
}
