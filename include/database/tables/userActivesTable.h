#pragma once

#include "database/tables/baseTable.h"
//#include "database.h"
//#include "logger.h"
//#include "preparedStatementManager.h"

#include <string>
#include <memory>
#include <pqxx/pqxx>

class Database;

class UserActivesTable : BaseTable {
public:
	enum onlineStatus {
		online,
		offline
	};
private:
	std::shared_ptr<Database> _Database;

	/*Database* _Database;
	Logger* _Logger;
	PreparedStatementManager* _PreparedStatementManager;*/


	const std::string MODULE_NAME_ = "(Table UserActives)";
	// pqxx::work txn;
public:
	UserActivesTable();
	~UserActivesTable() = default;

	// statements
	void initStatements() override;

	// getters
	std::string getOnlineStatusByUID(uint64_t& UID);

	// setters
	void setOnlineStatusByUID(uint64_t& UID, onlineStatus);

};