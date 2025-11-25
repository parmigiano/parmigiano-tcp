#include "database/tables/messageStatusesTable.h"

#include "database/database.h"

MessageStatusesTable::MessageStatusesTable() {
	_Database = std::make_shared<Database>();

	initStatements();
}

void MessageStatusesTable::initStatements() {
	_PreparedStatementManager->registerStatement("addMessageStatuses", R"(INSERT INTO message_statuses (message_id, receiver_uid) 
																	VALUES ($1, $2)
																	RETURNING delivered_at;)");
	
	_PreparedStatementManager->registerStatement("updateReadAt", R"(UPDATE message_statuses 
																	SET read_at = timezone('UTC', now()) 
																	WHERE message_id = $1 
																	AND receiver_uid = $2;)");

	_PreparedStatementManager->registerStatement("getDeliveredAt", R"(SELECT DISTINCT delivered_at
																	FROM message_statuses
																	WHERE message_id = $1;)");
}

std::string MessageStatusesTable::addMessage(uint64_t& message_id, uint64_t& receiver_uid) {
	try {
		//std::vector <uint64_t> list_of_uid;
		std::shared_ptr<pqxx::connection> _Connection = _Database->getConnection();
		pqxx::work transaction(*_Connection);

		//for (const uint64_t& interlocutor_uid : interlocutors) {
		pqxx::result txn_result = _PreparedStatementManager->exec(transaction, "addMessageStatuses", message_id, receiver_uid);
		//}

		std::string delivered_at = txn_result[0][0].as<std::string>();

		transaction.commit();

		return delivered_at;
	}
	catch (const std::exception& error) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " " + std::to_string(__LINE__) + " except: " + std::string(error.what()), 2);
		return std::string();
	}
	catch (...) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " " + std::to_string(__LINE__) + " catch unknw error", 2);
		return std::string();
	}
}

void MessageStatusesTable::updateReadAt(uint64_t& message_id, uint64_t& receiver_uid) {
	try {
		std::shared_ptr<pqxx::connection> _Connection = _Database->getConnection();
		pqxx::work transaction(*_Connection);

		_PreparedStatementManager->exec(transaction, "updateReadAt", message_id, receiver_uid);

		transaction.commit();
	}
	catch (const std::exception& error) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " " + std::to_string(__LINE__) + " except: " + std::string(error.what()), 2);
	}
	catch (...) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " " + std::to_string(__LINE__) + " catch unknw error", 2);
	}
}

std::string MessageStatusesTable::getDeliveredAt(uint64_t& message_id) {
	try {
		std::shared_ptr<pqxx::connection> _Connection = _Database->getConnection();
		pqxx::work transaction(*_Connection);

		pqxx::result txn_result = _PreparedStatementManager->exec(transaction, "getDeliveredAt", message_id);

		std::string delivered_at = txn_result[0][0].as<std::string>();

		transaction.commit();

		return delivered_at;
	}
	catch (const std::exception& error) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " " + std::to_string(__LINE__) + " except: " + std::string(error.what()), 2);
		return std::string();
	}
	catch (...) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " " + std::to_string(__LINE__) + " catch unknw error", 2);
		return std::string();
	}
}
