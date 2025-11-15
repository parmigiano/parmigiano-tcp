#pragma once

#include "database/tables/baseTable.h"
//#include "database.h"
//#include "logger.h"
//#include "preparedStatementManager.h"

#include <string>
#include <memory>
#include <vector>
#include <pqxx/pqxx>

class ChatMembersTable : BaseTable {
public:
private:
	/*Database* _Database;
	Logger* _Logger;
	PreparedStatementManager* _PreparedStatementManager;*/

	const std::string MODULE_NAME_ = "(Table ChatMembers)";
public:
	ChatMembersTable();
	~ChatMembersTable() = default;

	// statements
	void initStatements() override;
	 
	// getters;
	std::vector<uint64_t> getListOfAllInterlocutors(uint64_t& UID);
	std::vector<uint64_t> getListOfChatMembers(uint64_t& UID, uint64_t& chat_id);

	// setters
}; 