#pragma once

#include "logger.h"

#include <string>
#include <vector>
#include <mutex>
#include <map>
#include <chrono>
#include <variant>
#include <boost/asio.hpp>

class Session;

struct SessionInfo {
	std::variant<uint64_t, Session*> variant;
	std::chrono::time_point<std::chrono::system_clock> lastActivity;

	SessionInfo& operator=(const SessionInfo& other) {
		variant = other.variant;
		lastActivity = other.lastActivity;
		return *this;
	}
};

class SessionManager {
private:
	Logger* _Logger;

	static SessionManager* instance_ptr_;
	static std::mutex instance_mtx_;
	std::mutex mtx_;

	std::map<Session*, SessionInfo> users_identify_by_session_;
	std::map<uint64_t, SessionInfo> users_identify_by_uid_;

	std::vector<uint64_t> list_of_uid_;

	const std::string MODULE_NAME_ = "(SessionManager)";
public:
	SessionManager();
	SessionManager(const SessionManager&) = delete;
	~SessionManager() = default;

	static SessionManager* get_instance();

	void assigningSession(Session& session, uint64_t& UID); // adding new session

	//void updateSession(Session& session, uint64_t& UID);

	//void updateLastActivity(uint64_t& UID);
	void updateLastActivity(Session& session);

	//bool sessionExist(uint64_t& UID);
	bool sessionExist(Session& session);
	bool sessionExist(uint64_t& UID);

	//std::chrono::time_point<std::chrono::system_clock> getSessionLastActivity(uint64_t& UID);
	std::chrono::time_point<std::chrono::system_clock> getSessionLastActivity(Session& session);
	std::chrono::time_point<std::chrono::system_clock> getSessionLastActivity(uint64_t& UID);

	uint64_t getSessionUID(Session& session);
	Session* getSessionByUID(uint64_t& UID);
	//bool sessionExistByUID(uint64_t& UID);

	//boost::asio::ip::tcp::socket& getSessionSocket(uint64_t& UID);
	//boost::asio::ip::tcp::socket& getSessionSocket(Session& session);

	std::vector<uint64_t> getListOfUID();
	//uint64_t getUIDbySocket(boost::asio::ip::tcp::socket& client_socket);

	//void removeSession(uint64_t& UID);
	void removeSession(Session& session);
};