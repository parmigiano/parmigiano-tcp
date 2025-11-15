#include "session/sessionManager.h"

#include <random>

SessionManager* SessionManager::instance_ptr_ = nullptr;
std::mutex SessionManager::instance_mtx_;

SessionManager::SessionManager() {
	_Logger = Logger::get_instance();
}

SessionManager* SessionManager::get_instance() {
	std::lock_guard<std::mutex> lock(instance_mtx_);
	if (instance_ptr_ == nullptr) {
		instance_ptr_ = new SessionManager();
	}

	return instance_ptr_;
}

bool SessionManager::sessionExist(uint64_t& UID) {
	if (users_identify_by_uid_.find(UID) != users_identify_by_uid_.end()) {
		return true;
	}

	return false;
}

void SessionManager::assigningSession(boost::asio::ip::tcp::socket& socket, uint64_t& UID) {
	try {
		std::lock_guard<std::mutex> lock(mtx_);

		if (!sessionExist(UID)) {
			users_identify_by_uid_[UID] = SessionInfo{ &socket, std::chrono::system_clock::now() };
		}
	}
	catch (const std::exception& error) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
	}
	catch (...) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
	}
}

void SessionManager::updateSocket(boost::asio::ip::tcp::socket& socket, uint64_t& UID) {
	std::lock_guard<std::mutex> lock(mtx_);
	if (sessionExist(UID)) {
		users_identify_by_uid_[UID].socket = &socket;
	}
}

void SessionManager::updateLastActivity(uint64_t& UID) {
	std::lock_guard<std::mutex> lock(mtx_);
	if (sessionExist(UID)) {
		users_identify_by_uid_[UID].lastActivity = std::chrono::system_clock::now();
	}
}

std::chrono::time_point<std::chrono::system_clock> SessionManager::getSessionLastActivity(uint64_t& UID) {
	if (sessionExist(UID)) {
		return users_identify_by_uid_[UID].lastActivity;
	}

	throw std::runtime_error("there is no active session with this UID");
}

boost::asio::ip::tcp::socket& SessionManager::getSessionSocket(uint64_t& UID) {
	if (sessionExist(UID)) {
		return *users_identify_by_uid_[UID].socket;
	}

	throw std::runtime_error("there is no active session with this UID");
}

std::vector<uint64_t> SessionManager::getListOfUID() {
	list_of_uid_.clear();

	for (auto& session : users_identify_by_uid_) {
		list_of_uid_.push_back(session.first);
	}

	return list_of_uid_;
}

uint64_t SessionManager::getUIDbySocket(boost::asio::ip::tcp::socket& client_socket) {
	uint64_t UID = 0;

	if (list_of_uid_.empty()) {
		throw std::runtime_error("Server havent any session");
	}

	for (uint64_t& user_id : list_of_uid_) {
		boost::asio::ip::tcp::socket& sock = getSessionSocket(user_id);
		if (&sock == &client_socket) {
			UID = user_id;
		}
	}

	if (UID == 0) {
		throw std::runtime_error("Dont find user: " + std::to_string(UID) + " in sessions");
	}

	return UID;
}

void SessionManager::removeClientFromTable(uint64_t& UID) {
	std::lock_guard<std::mutex> lock(mtx_);
	if (sessionExist(UID)) {
		users_identify_by_uid_.erase(UID);
	}
}
