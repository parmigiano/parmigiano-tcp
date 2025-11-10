#include "database/tables/messagesTable.h"

MessagesTable::MessagesTable() {
	/*_Logger = Logger::get_instance();
	_PreparedStatementManager = PreparedStatementManager::get_instance();*/
}

void MessagesTable::initStatements() {
	_PreparedStatementManager->registerStatement("getContent", "FROM messages SELECT content WHERE chat_id = $1");
}