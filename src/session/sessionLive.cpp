#include "session/sessionLive.h"

//#include "../include/sendResponse.h"
#include "connect_processing/clientShutdown.h"

#include <vector>

SessionLive::SessionLive() {
	_Logger = Logger::get_instance();
	_Config = Config::get_instance();
    _SessionManager = SessionManager::get_instance();

	//_SendResponse = std::make_shared<SendResponse>();
	_ClientShutdown = std::make_shared<ClientShutdown>();
}

void SessionLive::checkActivity() {
    try {
        /*std::vector list_of_uid_ = _SessionManager->getListOfUID();
        auto sessionLiveTime = std::chrono::minutes(std::stoi(_Config->configuration_vars_["sessionLiveTime"]));*/

        /*for (auto& UID : list_of_uid_) {
            auto now = std::chrono::system_clock::now();
            auto sessionLastActivity = _SessionManager->getSessionLastActivity(UID);

            if (now - sessionLastActivity >= sessionLiveTime) {
                _SessionManager->removeSession(UID);

                _ClientShutdown->disconnectClientByReason(
                    UID, 
                    "the last session activity was more than" + _Config->configuration_vars_["sessionLiveTime"] + " minutes ago", 
                    0, 
                    inactive
                );
            }
        }*/

        //list_of_uid_.clear();
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}