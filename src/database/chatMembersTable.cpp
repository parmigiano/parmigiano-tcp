#include "database/tables/chatMembersTable.h"

ChatMembersTable::ChatMembersTable() {
	/*_Database = Database::get_instance();
	_Logger = Logger::get_instance();
	_PreparedStatementManager = PreparedStatementManager::get_instance();*/
}

void ChatMembersTable::initStatements() {
    _PreparedStatementManager->registerStatement("getListOfInterlocutors", R"(SELECT DISTINCT cm2.user_id
																			FROM chat_members cm1
																			JOIN chat_members cm2 ON cm1.chat_id = cm2.chat_id
																			WHERE cm1.user_id = $1 AND cm2.user_id <> $2)");
}

std::vector<uint64_t> ChatMembersTable::getListOfInterlocutors(uint64_t& UID) {
	try {
		std::vector <uint64_t> list_of_uid;
		std::shared_ptr<pqxx::connection> conn = _Database->getConnection();
		pqxx::work transaction(*conn);

		pqxx::result txn_result = _PreparedStatementManager->exec(transaction, "getListOfInterlocutors", UID);

		for (const pqxx::row& row : txn_result) {
			list_of_uid.push_back(row[0].as<uint64_t>());
		}

		transaction.commit();

		return list_of_uid;
	}
	catch (const std::exception& error) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
		return std::vector<uint64_t>();
	}
	catch (...) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
		return std::vector<uint64_t>();
	}
}
