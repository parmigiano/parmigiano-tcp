#pragma once

#include "database/tables/baseTable.h"
//#include "logger.h"
//#include "preparedStatementManager.h"

#include <string>
#include <memory>
#include <pqxx/pqxx>

class MessagesTable : BaseTable {
private:
	/*Logger* _Logger;
	PreparedStatementManager* _PreparedStatementManager;*/
public:
	MessagesTable();
	~MessagesTable() = default;

	// init statements
	void initStatements() override;

	// getters

	// setters
};