#include "database/tables/messageEditsTable.h"

#include "database/database.h"

MessageEditsTable::MessageEditsTable() {
	_Database = std::make_shared<Database>();

	initStatements();
}

void MessageEditsTable::initStatements() {
	_PreparedStatementManager->registerStatement("addNewMeesageEdit", R"(INSERT INTO message_edits (message_id, old_content, new_content, editor_uid)
																		VALUES ($1, $2, $3, $4);)");
}

void MessageEditsTable::addNewMeesageEdit(uint64_t& message_id, std::string& old_content, std::string& new_content, uint64_t& editor_uid) {
    try {
        std::shared_ptr<pqxx::connection> _Connection = _Database->getConnection();
        pqxx::work transaction(*_Connection);

        pqxx::result txn_result = _PreparedStatementManager->exec(transaction, "addNewMeesageEdit", message_id, old_content, new_content, editor_uid);

        transaction.commit();
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}
