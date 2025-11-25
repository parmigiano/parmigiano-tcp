#include "connect_processing/userStatusService.h"

#include "connect_processing/sendResponse.h"
#include "database/tables/chatMembersTable.h"
#include "database/tables/userActivesTable.h"
#include "session/session.h"

UserStatusService::UserStatusService() {
	_Logger = Logger::get_instance();
	_SessionManager = SessionManager::get_instance();

	_SendResponse = std::make_shared<SendResponse>();
	_ChatMembers = std::make_shared<ChatMembersTable>();
	_UserActives = std::make_shared<UserActivesTable>();
}

void UserStatusService::notifyOffline(Session& session) {
	uint64_t UID;

	try {
		UID = _SessionManager->getSessionUID(session);
		std::vector <uint64_t> interlocutors = _ChatMembers->getListOfAllInterlocutors(UID);
		
		_UserActives->setOnlineStatusByUID(UID, _UserActives->offline);

		_Logger->addSessionLog(_Logger->info, UID, "Status changed to offline", 0);

		if (interlocutors.empty()) {
			return;
		}

		for (uint64_t& interlocutor_uid : interlocutors) {
			bool interlocutor_session_active = _SessionManager->sessionExist(interlocutor_uid);

			if (!interlocutor_session_active) {
				continue;
			}

			Session* session = _SessionManager->getSessionByUID(interlocutor_uid);

			// notify that one of the interlocutors is offline
			_SendResponse->setClientActiveInfo(interlocutor_uid, false);
			_SendResponse->setResponseType(user_online_status);

			_SendResponse->sendResponse(*session);
		}
	}
	catch (const std::exception& error) {
		_Logger->addSessionLog(_Logger->warn, UID, MODULE_NAME_ + " except: " + std::string(error.what()), 0);
	}
	catch (...) {
		_Logger->addSessionLog(_Logger->warn, UID, MODULE_NAME_ + " catch unknw error", 0);
	}
}

void UserStatusService::notifyOnline(Session& session) {
	uint64_t UID;

	try {
		UID = _SessionManager->getSessionUID(session);
		std::vector <uint64_t> interlocutors = _ChatMembers->getListOfAllInterlocutors(UID);

		_UserActives->setOnlineStatusByUID(UID, _UserActives->online);

		_Logger->addSessionLog(_Logger->info, UID, "Status changed to online", 0);

		if (interlocutors.empty()) {
			return;
		}

		for (uint64_t& interlocutor_uid : interlocutors) {
			bool interlocutor_session_active = _SessionManager->sessionExist(interlocutor_uid);
			Session* session = _SessionManager->getSessionByUID(interlocutor_uid);

			if (!interlocutor_session_active) {
				continue;
			}

			// notify that one of the interlocutors is online
			_SendResponse->setResponseType(user_online_status);
			_SendResponse->setClientActiveInfo(interlocutor_uid, true);

			_SendResponse->sendResponse(*session);
		}

		//_SessionManager->updateLastActivity(UID);
	}
	catch (const std::exception& error) {
		_Logger->addSessionLog(_Logger->warn, UID, MODULE_NAME_ + " except: " + std::string(error.what()), 0);
	}
	catch (...) {
		_Logger->addSessionLog(_Logger->warn, UID, MODULE_NAME_ + " catch unknw error", 0);
	}
}
