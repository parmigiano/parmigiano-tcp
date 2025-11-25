#pragma once

#include "database/tables/baseTable.h"
//#include "logger.h"
//#include "preparedStatementManager.h"

#include <string>
#include <vector>
#include <memory>
#include <pqxx/pqxx>

class Database;

class MessageStatusesTable : BaseTable {
private:
	std::shared_ptr<Database> _Database;

	const std::string MODULE_NAME_ = "(Table MessageStatuses)"; 
public:
	MessageStatusesTable();
	~MessageStatusesTable() = default;

	void initStatements() override;

	std::string addMessage(uint64_t& message_id, uint64_t& receiver_uid);
	void updateReadAt(uint64_t& message_id, uint64_t& receiver_uid);
	std::string getDeliveredAt(uint64_t& message_id);
};