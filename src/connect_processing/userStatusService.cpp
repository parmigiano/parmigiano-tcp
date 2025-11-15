#include "connect_processing/userStatusService.h"

#include "connect_processing/sendResponse.h"
#include "database/tables/chatMembersTable.h"
#include "database/tables/userActivesTable.h"

UserStatusService::UserStatusService() {
	_Logger = Logger::get_instance();
	_SessionManager = SessionManager::get_instance();

	//std::shared_ptr<SendResponse> _SendResponse;
	//std::shared_ptr<ChatMembers> _ChatMembers;
	//std::shared_ptr<UserActives> _UserActives;

	_SendResponse = std::make_shared<SendResponse>();
	_ChatMembers = std::make_shared<ChatMembersTable>();
	_UserActives = std::make_shared<UserActivesTable>();
}

void UserStatusService::notifyOffline(boost::asio::ip::tcp::socket& client_socket) {
	try {
		uint64_t UID = _SessionManager->getUIDbySocket(client_socket);

		notifyOffline(UID);
	}
	catch (const std::exception& error) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
	}
	catch (...) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
	}
}

void UserStatusService::notifyOffline(uint64_t& UID) {
	try {
		std::vector <uint64_t> interlocutors = _ChatMembers->getListOfAllInterlocutors(UID);

		if (interlocutors.empty()) {
			_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " User: " + std::to_string(UID)+ " havent interlocutors", 2);
		}

		for (uint64_t& interlocutor_uid : interlocutors) {
			boost::asio::ip::tcp::socket& sock = _SessionManager->getSessionSocket(interlocutor_uid);
			// notify that one of the participants is offline
			_SendResponse->setClientActiveInfo(interlocutor_uid, false);
			_SendResponse->setResponseType(user_online_status);

			_SendResponse->sendResponse(sock);
		}

		_UserActives->setOnlineStatusByUID(UID, _UserActives->offline);
		_SessionManager->removeClientFromTable(UID);
	}
	catch (const std::exception& error) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
	}
	catch (...) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
	}
}

void UserStatusService::notifyOnline(boost::asio::ip::tcp::socket& client_socket) {
	try {
		uint64_t UID = _SessionManager->getUIDbySocket(client_socket);

		notifyOffline(UID);
	}
	catch (const std::exception& error) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
	}
	catch (...) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
	}
}

void UserStatusService::notifyOnline(uint64_t& UID) {
	try {
		std::vector <uint64_t> interlocutors = _ChatMembers->getListOfAllInterlocutors(UID);

		for (uint64_t& interlocutor_uid : interlocutors) {
			boost::asio::ip::tcp::socket& sock = _SessionManager->getSessionSocket(interlocutor_uid);
			// notify that one of the participants is online
			_SendResponse->setResponseType(user_online_status);
			_SendResponse->setClientActiveInfo(interlocutor_uid, true);

			_SendResponse->sendResponse(sock);
		}

		//_SessionManager->updateLastActivity(UID);
		_UserActives->setOnlineStatusByUID(UID, _UserActives->online);
	}
	catch (const std::exception& error) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
	}
	catch (...) {
		_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
	}
}
