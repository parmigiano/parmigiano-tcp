#ifndef BASE_H
#define BASE_H

#include "database.h"
#include "logger.h"
#include "preparedStatementManager.h"

#include <string>
#include <memory>
#include <pqxx/pqxx>

class Base {
public:
private:
	Database* _Database;
	Logger* _Logger;
	PreparedStatementManager* _PreparedStatementManager;
public:
	Base();
	~Base() = default;

	void initStatements();
};

#endif 