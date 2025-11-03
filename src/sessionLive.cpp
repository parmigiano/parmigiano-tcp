#include "../include/sessionLive.h"

//#include "../include/sendResponse.h"
#include "../include/forcedClientShutdown.h"

#include <vector>

SessionLive::SessionLive() {
	_Logger = Logger::get_instance();
	_Config = Config::get_instance();
    _SessionManager = SessionManager::get_instance();

	//_SendResponse = std::make_shared<SendResponse>();
	_ForcedClientShutdown = std::make_shared<ForcedClientShutdown>();
}

void SessionLive::checkActivity() {
    try {
        std::vector listOfUID = _SessionManager->getListOfUID();
        auto sessionLiveTime = std::chrono::minutes(std::stoi(_Config->configurationVars["sessionLiveTime"]));

        for (auto& UID : listOfUID) {
            auto now = std::chrono::system_clock::now();
            auto sessionLastActivity = _SessionManager->getSessionLastActivity(UID);

            if (now - sessionLastActivity >= sessionLiveTime) {
                _SessionManager->removeClientFromTable(UID);

                _ForcedClientShutdown->disconnectClientByReason(
                    UID, 
                    "the last session activity was more than" + _Config->configurationVars["sessionLiveTime"] + " minutes ago", 
                    0, 
                    _ForcedClientShutdown->inactive
                );
            }
        }
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, "(sessionLive) except: " + std::atoi(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, "(sessionLive) catch unknw error", 2);
    }
}