#pragma once

#include "database/tables/baseTable.h"
//#include "logger.h"
//#include "preparedStatementManager.h"

#include <string>
#include <memory>
#include <pqxx/pqxx>

class Database;

class MessageEditsTable : BaseTable {
private:
	std::shared_ptr<Database> _Database;

	const std::string MODULE_NAME_ = "(Table MessageEdits)";
public:
	MessageEditsTable();
	~MessageEditsTable() = default;

	void initStatements() override;

	void addNewMeesageEdit(uint64_t& message_id, std::string& old_content, std::string& new_content, uint64_t& editor_uid);
};