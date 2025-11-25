#include "session/sessionManager.h"

#include <random>
#include <session/session.h>

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

//bool SessionManager::sessionExist(uint64_t& UID) {
//	if (users_identify_by_uid_.find(UID) != users_identify_by_uid_.end()) {
//		return true;
//	}
//
//	return false;
//}

bool SessionManager::sessionExist(Session& session) {
	if (users_identify_by_session_.find(&session) != users_identify_by_session_.end()) {
		return true;
	}

	return false;
}

bool SessionManager::sessionExist(uint64_t& UID) {
	if (users_identify_by_uid_.find(UID) != users_identify_by_uid_.end()) {
		return true;
	}

	return false;
}

void SessionManager::assigningSession(Session& session, uint64_t& UID) {
	try {
		std::lock_guard<std::mutex> lock(mtx_);

		if (!sessionExist(session)) {
			/*users_identify_by_uid_[UID] = { &session, std::chrono::system_clock::now() };
			users_identify_by_session_[session] = { &session, std::chrono::system_clock::now() };*/

			users_identify_by_session_[&session] = { UID, std::chrono::system_clock::now() };
			users_identify_by_uid_[UID] = { &session, std::chrono::system_clock::now() };
			//list_of_uid_.push_back(UID);
		}
	}
	catch (const std::exception& error) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
	}
	catch (...) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
	}
}

//void SessionManager::updateSession(Session& session, uint64_t& UID) {
//	std::lock_guard<std::mutex> lock(mtx_);
//
//	if (sessionExist(session)) {
//		users_identify_by_session_[session].session = &session;
//		//users_identify_by_uid_[UID].socket = std::move(socket);
//	}
//}

void SessionManager::updateLastActivity(Session& session) {
	std::lock_guard<std::mutex> lock(mtx_);

	if (sessionExist(session)) {
		users_identify_by_session_[&session].lastActivity = std::chrono::system_clock::now();
	}
}

//void SessionManager::updateLastActivity(uint64_t& UID) {
//	std::lock_guard<std::mutex> lock(mtx_);
//
//	if (sessionExist(UID)) {
//		users_identify_by_uid_[UID].lastActivity = std::chrono::system_clock::now();
//	}
//}

//void SessionManager::updateLastActivity(uint64_t& UID) {
//
//}
//
//std::chrono::time_point<std::chrono::system_clock> SessionManager::getSessionLastActivity(uint64_t& UID) {
//	if (sessionExist(UID)) {
//		return users_identify_by_uid_[UID].lastActivity;
//	}
//
//	//throw std::runtime_error("there is no active session with this UID" + UID);
//}

//boost::asio::ip::tcp::socket& SessionManager::getSessionSocket(uint64_t& UID) {
//	if (sessionExist(UID)) {
//		return users_identify_by_uid_[UID].session->socket();
//	}
//
//	//throw std::runtime_error("there is no active session with this UID: " + UID);
//}

std::chrono::time_point<std::chrono::system_clock> SessionManager::getSessionLastActivity(Session& session) {
	if (sessionExist(session)) {
		return users_identify_by_session_[&session].lastActivity;
	}

	return std::chrono::time_point<std::chrono::system_clock>();
}

std::chrono::time_point<std::chrono::system_clock> SessionManager::getSessionLastActivity(uint64_t& UID) {
	if (sessionExist(UID)) {
		return users_identify_by_uid_[UID].lastActivity;
	}

	return std::chrono::time_point<std::chrono::system_clock>();
}

uint64_t SessionManager::getSessionUID(Session& session) {
	if (sessionExist(session)) {
		uint64_t UID = std::get<uint64_t>(users_identify_by_session_[&session].variant);

		return UID;
	}

	return 0;
}

Session* SessionManager::getSessionByUID(uint64_t& UID) {
	if (sessionExist(UID)) {
		Session* session = std::get<Session*>(users_identify_by_uid_[UID].variant);

		return session;
		/*uint64_t UID = std::get<uint64_t>(users_identify_by_session_[session].variant);

		return UID;*/
	}

	return 0;
}

//boost::asio::ip::tcp::socket& SessionManager::getSessionSocket(Session& session) {
//
//	// TODO: вставьте здесь оператор return
//}

std::vector<uint64_t> SessionManager::getListOfUID() {
	list_of_uid_.clear();

	for (auto& session_info : users_identify_by_session_) {
		list_of_uid_.push_back(std::get<uint64_t>(session_info.second.variant));
	}

	/*if (!list_of_uid_.empty()) {
		return list_of_uid_;
	}*/

	return std::vector<uint64_t>();
}

//void SessionManager::removeSession(uint64_t& UID) {
//	std::lock_guard<std::mutex> lock(mtx_);
//
//	if (sessionExist(UID)) {
//		Session& session = *users_identify_by_uid_[UID].session;
//
//		users_identify_by_uid_.erase(UID);
//		users_identify_by_session_.erase(session);
//	}
//}

void SessionManager::removeSession(Session& session) {
	std::lock_guard<std::mutex> lock(mtx_);

	if (sessionExist(session)) {
		uint64_t UID = std::get<uint64_t>(users_identify_by_session_[&session].variant);

		users_identify_by_session_.erase(&session);
		users_identify_by_uid_.erase(UID);

		/*auto it = std::find(list_of_uid_.begin(), list_of_uid_.end(), UID);
		if (it != list_of_uid_.end()) {
			list_of_uid_.erase(it);
		}*/
	}
}
