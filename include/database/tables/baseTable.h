#pragma once

#include "database/database.h"
#include "logger.h"
#include "database/preparedStatementManager.h"

#include <string>
#include <memory>
#include <pqxx/pqxx>

class BaseTable {
protected:
	Database* _Database;
	Logger* _Logger;
	PreparedStatementManager* _PreparedStatementManager;
public:
	BaseTable();
	virtual ~BaseTable() = default;

	virtual void initStatements() = 0;
};