#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "logger.h"

#include <string>
#include <vector>
#include <mutex>
#include <map>
#include <chrono>
#include <boost/asio.hpp>

struct SessionInfo {
	boost::asio::ip::tcp::socket* socket;
	std::chrono::time_point<std::chrono::system_clock> lastActivity;

	SessionInfo& operator=(const SessionInfo& info) {
		socket = info.socket;
		lastActivity = info.lastActivity;
		return *this;
	}
};

class SessionManager {
private:
	Logger* _Logger;

	static SessionManager* instance_ptr_;
	static std::mutex instance_mtx_;
	std::mutex mtx_;

	std::map<uint64_t, SessionInfo> users_identify_by_uid_;
	std::vector<uint64_t> list_of_uid_;

	bool sessionExist(uint64_t& UID);

	const std::string MODULE_NAME_ = "(SessionManager)";
public:
	SessionManager();
	SessionManager(const SessionManager&) = delete;
	~SessionManager() = default;

	static SessionManager* get_instance();

	void assigningSession(boost::asio::ip::tcp::socket& client_socket, uint64_t& UID); // adding new session

	void updateSocket(boost::asio::ip::tcp::socket& client_socket, uint64_t& UID);
	void updateLastActivity(uint64_t& UID);

	std::chrono::time_point<std::chrono::system_clock> getSessionLastActivity(uint64_t& UID);
	boost::asio::ip::tcp::socket& getSessionSocket(uint64_t& UID);
	std::vector<uint64_t> getListOfUID();

	void removeClientFromTable(uint64_t& UID);
};

#endif 