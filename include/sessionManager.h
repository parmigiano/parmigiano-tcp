#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "../include/logger.h"

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

	static SessionManager* instance_ptr;
	static std::mutex instance_mtx;
	std::mutex mtx;

	std::map <std::string, SessionInfo> _UsersIdentify;
	std::vector<std::string> listOfUID;

	bool sessionExist(std::string& UID);
public:
	SessionManager();
	SessionManager(const SessionManager&) = delete;
	~SessionManager() = default;

	static SessionManager* get_instance();

	void assigningSession(boost::asio::ip::tcp::socket& socket, std::string& UID); // adding new session
	void updateSocket(boost::asio::ip::tcp::socket& socket, std::string& UID);
	void updateLastActivity(std::string& UID);
	std::chrono::time_point<std::chrono::system_clock> getSessionLastActivity(std::string& UID);
	boost::asio::ip::tcp::socket* getSessionSocket(std::string& UID);
	std::vector<std::string> getListOfUID();
	void removeClientFromTable(std::string& UID);
};

#endif 