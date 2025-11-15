#pragma once

#include "database/tables/baseTable.h"
//#include "database.h"
//#include "logger.h"
//#include "preparedStatementManager.h"

#include <string>
#include <memory>
#include <pqxx/pqxx>

class ChatsTable : BaseTable {
private:
	const std::string MODULE_NAME_ = "(Table Chats)";
public:
	ChatsTable();
	~ChatsTable() = default;

	// statements
	void initStatements() override;
};