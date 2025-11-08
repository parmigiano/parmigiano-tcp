#ifndef CHATMEMBERS_H
#define CHATMEMBERS_H

#include "database.h"
#include "logger.h"
#include "preparedStatementManager.h"

#include <string>
#include <memory>
#include <vector>
#include <pqxx/pqxx>

class ChatMembers {
public:
private:
	Database* _Database;
	Logger* _Logger;
	PreparedStatementManager* _PreparedStatementManager;

	const std::string MODULE_NAME_ = "(Table Chats)";
public:
	ChatMembers();
	~ChatMembers() = default;

	// statements
	void initStatements();
	 
	// getters;
	std::vector<uint64_t> getListOfInterlocutors(uint64_t& uid);

	// setters
};

#endif 