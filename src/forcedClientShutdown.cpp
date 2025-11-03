#include "../include/forcedClientShutdown.h"

#include "../include/sendResponse.h"

ForcedClientShutdown::ForcedClientShutdown() {
	_Logger = Logger::get_instance();
	_Config = Config::get_instance();
    _SessionManager = SessionManager::get_instance();

	_SendResponse = std::make_shared<SendResponse>();
}

void ForcedClientShutdown::disconnect(std::string& UID) {
    boost::asio::ip::tcp::socket& socket = *_SessionManager->getSessionSocket(UID);

    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    socket.close();
}

void ForcedClientShutdown::notify(std::string& UID, std::string description, short int code, disconnectType& type) {
    boost::asio::ip::tcp::socket& socket = *_SessionManager->getSessionSocket(UID);

    _SendResponse->setDisconnectDescription(description);
    _SendResponse->setDisconnectCode(code);

    switch (type) {
    case ForcedClientShutdown::error:
        _SendResponse->setDisonnectType(_SendResponse->error);
        break;
    case ForcedClientShutdown::warn:
        _SendResponse->setDisonnectType(_SendResponse->warn);
        break;
    case ForcedClientShutdown::tempBan:
        _SendResponse->setDisonnectType(_SendResponse->tempBan);
        break;
    case ForcedClientShutdown::inactive:
        _SendResponse->setDisonnectType(_SendResponse->inactive);
        break;
    default:
        _SendResponse->setDisonnectType(_SendResponse->unknown);
        break;
    }

    _SendResponse->sendResponse(socket);
}

void ForcedClientShutdown::disconnectClientByReason(std::string& UID, std::string description, short int code, disconnectType type) {
    try {
        notify(UID, description, code, type);
        disconnect(UID);
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, "(forcedClientShutdown) except: " + std::atoi(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, "(forcedClientShutdown) catch unknw error", 2);
    }
}
