#ifndef DATABASE_H
#define DATABASE_H

#include "logger.h"
#include "config.h"

#include <mutex>
#include <memory>
#include <pqxx/pqxx>

class Database {
private:
	Logger* _Logger;
	Config* _Config;

	static Database* instance_ptr_;
	static std::mutex instance_mtx_;

	std::shared_ptr<pqxx::connection> conn_;
	std::string connection_string_;

	const std::string MODULE_NAME_ = "(Database)";
public:
	Database();
	Database(const Database&) = delete;
	~Database() = default;

	static Database* get_instance();

	void initialize(const std::string& conn_str);
	void createConnection();
	void diconnect();
	void reconnect();

	std::shared_ptr<pqxx::connection> getConnection();
};

#endif 