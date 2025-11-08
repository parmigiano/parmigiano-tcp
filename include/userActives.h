#ifndef USERACTIVES_H
#define USERACTIVES_H

#include "database.h"
#include "logger.h"
#include "preparedStatementManager.h"

#include <string>
#include <memory>
#include <pqxx/pqxx>

class UserActives {
public:
	enum onlineStatus {
		online,
		offline
	};
private:
	Database* _Database;
	Logger* _Logger;
	PreparedStatementManager* _PreparedStatementManager;

	const std::string MODULE_NAME_ = "(Table UserActives)";
	// pqxx::work txn;
public:
	UserActives();
	~UserActives() = default;

	// statements
	void initStatements();

	// getters
	std::string getOnlineStatusByUID(uint64_t& UID);

	// setters
	void setOnlineStatusByUID(uint64_t& UID, onlineStatus);

};

#endif 