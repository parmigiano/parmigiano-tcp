#pragma once

#include "database/tables/baseTable.h"
//#include "logger.h"
//#include "preparedStatementManager.h"

#include <string>
#include <memory>
#include <pqxx/pqxx>

class Database;

class MessagesTable : BaseTable {
private:
	std::shared_ptr<Database> _Database;

	/*Logger* _Logger;
	PreparedStatementManager* _PreparedStatementManager;*/
	const std::string MODULE_NAME_ = "(Table Messages)";
public:
	MessagesTable();
	~MessagesTable() = default;

	// init statements
	void initStatements() override;

	uint64_t addMessage(uint64_t& UID, uint64_t& chat_id, std::string& content, std::string& content_type);
	void updateIsPinned(uint64_t& chat_id, uint64_t& message_id);
	void updateIsEdited(uint64_t& chat_id, uint64_t& message_id);
	void deleteMessage(uint64_t& chat_id, uint64_t& message_id);
	std::map <std::string, std::string> getMessageInfo(uint64_t& message_id);
};