#include "database/tables/userActivesTable.h"

UserActivesTable::UserActivesTable() {
	/*_Database = Database::get_instance();
	_Logger = Logger::get_instance();
	_PreparedStatementManager = PreparedStatementManager::get_instance();*/
}

void UserActivesTable::initStatements() {
    _PreparedStatementManager->registerStatement("getOnlineStatusByUID", R"(SELECT online 
																			FROM user_actives 
																			WHERE user_uid = $1)");

    _PreparedStatementManager->registerStatement("setOnlineStatusByUID", R"(UPDATE user_actives 
																			SET online = $1 
																			WHERE user_uid = $2)");
}

std::string UserActivesTable::getOnlineStatusByUID(uint64_t& UID) {
	try {
		//std::shared_ptr<pqxx::connection> conn = _Database->getConnection();
		pqxx::work transaction(*_Connection);

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

void UserActivesTable::setOnlineStatusByUID(uint64_t& UID, onlineStatus status_type) {
	try {
		bool status;
		//std::shared_ptr<pqxx::connection> conn = _Database->getConnection();
		pqxx::work transaction(*_Connection);

		switch (status_type) {
		case UserActivesTable::online:
			status = true;
			break;
		case UserActivesTable::offline:
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
