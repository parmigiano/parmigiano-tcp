#include "../include/sessionManager.h"

#include <random>

SessionManager* SessionManager::instance_ptr = nullptr;
std::mutex SessionManager::instance_mtx;

SessionManager::SessionManager() {
	_Logger = Logger::get_instance();
}

SessionManager* SessionManager::get_instance() {
	std::lock_guard<std::mutex> lock(instance_mtx);
	if (instance_ptr == nullptr) {
		instance_ptr = new SessionManager();
	}

	return instance_ptr;
}

bool SessionManager::sessionExist(std::string& UID) {
	if (_UsersIdentify.find(UID) != _UsersIdentify.end()) {
		return true;
	}

	return false;
}

void SessionManager::assigningSession(boost::asio::ip::tcp::socket& socket, std::string& UID) {
	try {
		std::lock_guard<std::mutex> lock(mtx);

		if (!sessionExist(UID)) {
			_UsersIdentify[UID] = SessionInfo{ &socket, std::chrono::system_clock::now() };
		}
	}
	catch (const std::exception& error) {
		_Logger->addServerLog(_Logger->warn, "(sessionManager) except: " + std::atoi(error.what()), 2);
	}
	catch (...) {
		_Logger->addServerLog(_Logger->warn, "(sessionManager) catch unknw error", 2);
	}
}

void SessionManager::updateSocket(boost::asio::ip::tcp::socket& socket, std::string& UID) {
	std::lock_guard<std::mutex> lock(mtx);
	if (sessionExist(UID)) {
		_UsersIdentify[UID].socket = &socket;
	}
}

void SessionManager::updateLastActivity(std::string& UID) {
	std::lock_guard<std::mutex> lock(mtx);
	if (sessionExist(UID)) {
		_UsersIdentify[UID].lastActivity = std::chrono::system_clock::now();
	}
}

std::chrono::time_point<std::chrono::system_clock> SessionManager::getSessionLastActivity(std::string& UID) {
	if (sessionExist(UID)) {
		return _UsersIdentify[UID].lastActivity;
	}
}

boost::asio::ip::tcp::socket* SessionManager::getSessionSocket(std::string& UID) {
	if (sessionExist(UID)) {
		return _UsersIdentify[UID].socket;
	}
}

std::vector<std::string> SessionManager::getListOfUID() {
	listOfUID.clear();

	for (auto& session : _UsersIdentify) {
		listOfUID.push_back(session.first);
	}

	return listOfUID;
}

void SessionManager::removeClientFromTable(std::string& UID) {
	std::lock_guard<std::mutex> lock(mtx);
	if (sessionExist(UID)) {
		_UsersIdentify.erase(UID);
	}
}
