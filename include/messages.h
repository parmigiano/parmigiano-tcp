#ifndef MESSAGES_H
#define MESSAGES_H

#include "logger.h"
#include "preparedStatementManager.h"

#include <string>
#include <memory>
#include <pqxx/pqxx>

class Messages {
private:
	Logger* _Logger;
	PreparedStatementManager* _PreparedStatementManager;
public:
	Messages();
	~Messages() = default;

	// init statements
	void initStatements();

	// getters

	// setters
};

#endif 