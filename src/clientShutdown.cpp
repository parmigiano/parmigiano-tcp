#include "clientShutdown.h"

#include "sendResponse.h"

#include "vector"

СlientShutdown::СlientShutdown() {
	_Logger = Logger::get_instance();
	_Config = Config::get_instance();
    _SessionManager = SessionManager::get_instance();

	_SendResponse = std::make_shared<SendResponse>();
}

void СlientShutdown::disconnectCommon(boost::asio::ip::tcp::socket& client_socket) {
    client_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    client_socket.close();
}

void СlientShutdown::disconnect(uint64_t& UID) {
    boost::asio::ip::tcp::socket& client_socket = _SessionManager->getSessionSocket(UID);

    disconnectCommon(client_socket);
}

void СlientShutdown::disconnect(boost::asio::ip::tcp::socket& client_socket) {
    disconnectCommon(client_socket);
}

void СlientShutdown::notifyCommon(boost::asio::ip::tcp::socket& client_socket, std::string& description, short int& code, disconnectType& disconnect_type) {
    _SendResponse->setDisconnectDescription(description);
    _SendResponse->setDisconnectCode(code);

    switch (disconnect_type) {
    case СlientShutdown::error:
        _SendResponse->setDisonnectType(_SendResponse->error);
        break;
    case СlientShutdown::warn:
        _SendResponse->setDisonnectType(_SendResponse->warn);
        break;
    case СlientShutdown::tempBan:
        _SendResponse->setDisonnectType(_SendResponse->tempBan);
        break;
    case СlientShutdown::inactive:
        _SendResponse->setDisonnectType(_SendResponse->inactive);
        break;
    case СlientShutdown::littleInfo:
        _SendResponse->setDisonnectType(_SendResponse->inactive);
        break;
    default:
        throw std::runtime_error("Unknw disconnect type");
        break;
    }

    _SendResponse->sendResponse(client_socket);
}

void СlientShutdown::notify(uint64_t& UID, std::string description, short int code, disconnectType& disconnect_type) {
    boost::asio::ip::tcp::socket& client_socket = _SessionManager->getSessionSocket(UID);

    notifyCommon(client_socket, description, code, disconnect_type);
}

void СlientShutdown::notify(boost::asio::ip::tcp::socket& client_socket, std::string description, short int code, disconnectType& disconnect_type) {
    notifyCommon(client_socket, description, code, disconnect_type);
}

void СlientShutdown::disconnectClientByReason(uint64_t& UID, std::string description, short int code, disconnectType disconnect_type) {
    try {
        notify(UID, description, code, disconnect_type);
        disconnect(UID);
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void СlientShutdown::disconnectClientByReason(boost::asio::ip::tcp::socket& client_socket, std::string description, short int code, disconnectType disconnect_type) {
    try {
        disconnect(client_socket);
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}
