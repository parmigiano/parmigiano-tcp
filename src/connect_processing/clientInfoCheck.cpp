#include "connect_processing/clientInfoCheck.h"

#include "connect_processing/clientShutdown.h"

ClientInfoCheck::ClientInfoCheck() {
    _Logger = Logger::get_instance();
    //_SessionManager = SessionManager::get_instance();

    _ClientShutdown = std::make_shared<ClientShutdown>();
}

bool ClientInfoCheck::checkUID(uint64_t& UID, boost::asio::ip::tcp::socket& client_socket) {
    try {
        if (UID <= 0) {
            _ClientShutdown->disconnectClientByReason(client_socket, "", 0, _ClientShutdown->littleInfo); // namutit peregruzku shobi i po UID i po socket

            _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " Client havent UID and dont added to sessions table", 2);
            //throw std::runtime_error("Client havent UID and dont added to sessions table");
            return false;
        }
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
        return false;
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
        return false;
    }
}

bool ClientInfoCheck::checkInfoFullness(ClientRequestStruct::Request& accepted_request, boost::asio::ip::tcp::socket& client_socket) {
    try {
        uint64_t UID = accepted_request.mutable_clientinfo()->uid();

        if (checkUID(UID, client_socket)) {
            return true;
        }
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
        return false;
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
        return false;
    }
}
