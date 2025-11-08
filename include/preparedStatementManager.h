#ifndef PREPAREDSTATEMENTMANAGER_H
#define PREPAREDSTATEMENTMANAGER_H

#include "database.h"
#include "logger.h"

#include <string>
#include <unordered_map>
#include <memory>
#include <pqxx/pqxx>
#include <mutex>

class PreparedStatementManager {
private:
	Database* _Database;
	Logger* _Logger;

	static PreparedStatementManager* instance_ptr_;
	static std::mutex instance_mtx_;

	std::shared_ptr<pqxx::connection> conn_;
	std::unordered_map<std::string, std::string> prepared_;

	const std::string MODULE_NAME_ = "(PreparedStatementManager)";
public:
	PreparedStatementManager();
	PreparedStatementManager(const PreparedStatementManager&) = delete;
	~PreparedStatementManager() = default;

	static PreparedStatementManager* get_instance();

	void registerStatement(const std::string& name, const std::string& query);
	bool isRegistered(const std::string& name);
	void reprepareAll();
	std::string getSQL(const std::string& name);

	template <typename... Args>
	pqxx::result exec(pqxx::transaction_base& tb, const std::string& name, Args&&... args);
};

template <typename... Args>
pqxx::result PreparedStatementManager::exec(pqxx::transaction_base& tb, const std::string& name, Args&&... args) {
	try {
		if (!isRegistered(name)) {
			throw std::runtime_error("Unknown prepared statement name: " + name);
		}

		return tb.exec(pqxx::prepped(name), pqxx::params(std::forward<Args>(args)...));
	}
	catch (const std::exception& error) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
		return pqxx::result();
	}
	catch (...) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " unknown exception", 2);
		return pqxx::result();
	}
}

#endif 