#pragma once

#include "database/tables/baseTable.h"
//#include "logger.h"
//#include "preparedStatementManager.h"

#include <string>
#include <memory>
#include <pqxx/pqxx>

class MessageEditsTable : BaseTable {
private:
	const std::string MODULE_NAME_ = "(Table MessageEdits)";
public:
	MessageEditsTable();
	~MessageEditsTable() = default;
};