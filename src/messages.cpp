#include "messages.h"

Messages::Messages() {
	_Logger = Logger::get_instance();
	_PreparedStatementManager = PreparedStatementManager::get_instance();
}

void Messages::initStatements() {
	_PreparedStatementManager->registerStatement("getContent", "FROM messages SELECT content WHERE chat_id = $1");
}