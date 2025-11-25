#include "connect_processing/clientShutdown.h"

#include "connect_processing/sendResponse.h"
#include "session/session.h"

#include "vector"

ClientShutdown::ClientShutdown() {
	_Logger = Logger::get_instance();
	_Config = Config::get_instance();
    _SessionManager = SessionManager::get_instance();

	_SendResponse = std::make_shared<SendResponse>();
}

//void ClientShutdown::disconnectCommon(boost::asio::ip::tcp::socket& socket) {
//    boost::system::error_code ec;
//
//    socket.cancel(ec);
//    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
//    socket.close(ec);
//}
//
//void ClientShutdown::disconnect(uint64_t& UID) {
//    boost::asio::ip::tcp::socket& socket = _SessionManager->getSessionSocket(UID);
//
//    disconnectCommon(socket);
//}

void ClientShutdown::disconnect(Session* session) {
    boost::asio::ip::tcp::socket& client_socket = session->socket();

    boost::system::error_code ec;

    client_socket.cancel(ec);
    client_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    client_socket.close(ec);
}

//void ClientShutdown::notifyCommon(boost::asio::ip::tcp::socket& socket, std::string& description, short int& code, disconnectType& disconnect_type) {
//    _SendResponse->setDisonnectType(disconnect_type);
//    _SendResponse->setDisconnectInfo(description, code);
//
//    /*switch (disconnect_type) {
//    case ClientShutdown::error:
//        _SendResponse->setDisonnectType(_SendResponse->error);
//        break;
//    case ClientShutdown::warn:
//        _SendResponse->setDisonnectType(_SendResponse->warn);
//        break;
//    case ClientShutdown::tempBan:
//        _SendResponse->setDisonnectType(_SendResponse->tempBan);
//        break;
//    case ClientShutdown::inactive:
//        _SendResponse->setDisonnectType(_SendResponse->inactive);
//        break;
//    case ClientShutdown::littleInfo:
//        _SendResponse->setDisonnectType(_SendResponse->inactive);
//        break;
//    default:
//        throw std::runtime_error("Unknw disconnect type");
//        break;
//    }*/
//
//    _SendResponse->sendResponse(socket);
//}

//void ClientShutdown::notify(uint64_t& UID, std::string description, short int code, disconnectType& disconnect_type) {
//    std::shared_ptr<boost::asio::ip::tcp::socket> socket = std::make_shared<boost::asio::ip::tcp::socket>(std::move(_SessionManager->getSessionSocket(UID)));
//
//    notifyCommon(*socket, description, code, disconnect_type);
//}

void ClientShutdown::notify(Session* session, std::string description, short int code, disconnectType& disconnect_type) {
    //notifyCommon(socket, description, code, disconnect_type);

     _SendResponse->setDisonnectType(disconnect_type);
     _SendResponse->setDisconnectInfo(description, code);

    _SendResponse->sendResponse(*session);
}

//void ClientShutdown::disconnectClientByReason(uint64_t& UID, std::string description, short int code, disconnectType disconnect_type) {
//    try {
//        notify(UID, description, code, disconnect_type);
//        disconnect(UID);
//    }
//    catch (const std::exception& error) {
//        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
//    }
//    catch (...) {
//        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
//    }
//}

void ClientShutdown::disconnectClientByReason(Session* session, std::string description, short int code, disconnectType disconnect_type) {
    try {
        notify(session, description, code, disconnect_type);
        disconnect(session);
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}
