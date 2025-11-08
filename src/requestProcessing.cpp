#include "requestProcessing.h"

#include "sendResponse.h"
#include "userStatusNotify.h"
#include "clientInfoCheck.h"

RequestProcessing::RequestProcessing() {
    _Logger = Logger::get_instance();
    _Config = Config::get_instance();
    _SessionManager = SessionManager::get_instance();
    
    _SendResponse = std::make_shared<SendResponse>();
    _UserStatusNotify = std::make_shared<UserStatusNotify>();
    _ClientInfoCheck = std::make_shared<ClientInfoCheck>();
}

void RequestProcessing::requestDistribution(std::string request_str, boost::asio::ip::tcp::socket& client_socket) {
    try { 
        ClientRequestStruct::Request accepted_request;
        accepted_request.ParseFromString(request_str);

        if (!_ClientInfoCheck->checkInfoFullness(accepted_request, client_socket)) {
            throw std::runtime_error("Not all required data is filled");
        }

        ClientContext context{ accepted_request, client_socket };
        uint64_t UID = accepted_request.mutable_clientinfo()->uid();

        _SessionManager->assigningSession(client_socket, UID);

        switch (accepted_request.requestinfo().type()) { // replace to std::map<ClientRequestStruct::RequestInfo::..., std::function<void(ClientContext&)>>
        case ClientRequestStruct::RequestInfo::send_message:
            sendMessage(context);
            break;
        case ClientRequestStruct::RequestInfo::read_message:
            readMessage(context);
            break;
        case ClientRequestStruct::RequestInfo::edit_message:
            editMessage(context);
            break;
        case ClientRequestStruct::RequestInfo::pin_message:
            pinMessage(context);
            break;
        case ClientRequestStruct::RequestInfo::delete_message:
            deleteMessage(context);
            break;
        case ClientRequestStruct::RequestInfo::user_typing:
            userTyping(context);
            break;
        case ClientRequestStruct::RequestInfo::user_online_status:
            userOnlineStatus(context);
            break;
        case ClientRequestStruct::RequestInfo::get_unread_message_count:
            getUnreadMessageCount(context);
            break;
        default:
            //_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " unknw request type", 2);
            throw std::runtime_error("unknw request type");
            break;
        }
    } 
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void RequestProcessing::sendMessage(ClientContext& context) {

}

void RequestProcessing::readMessage(ClientContext& context) {

}

void RequestProcessing::editMessage(ClientContext& context) {

}

void RequestProcessing::pinMessage(ClientContext& context) {

}

void RequestProcessing::deleteMessage(ClientContext& context) {

}

void RequestProcessing::userTyping(ClientContext& context) {

}

void RequestProcessing::userOnlineStatus(ClientContext& context) {
    ClientRequestStruct::Request accepted_request = context.request;
    bool status = accepted_request.mutable_clientactivepacket()->online();
    uint64_t UID = static_cast<uint64_t>(accepted_request.mutable_clientactivepacket()->uid());
    //std::string uid_str = reinterpret_cast<const char*>(&uid);

    if (status == true) { // user is online now
        _UserStatusNotify->notifyOffline(UID);
    }
    else if (status == false) { // user is offline :((
        _UserStatusNotify->notifyOnline(UID);
    }
}

void RequestProcessing::getUnreadMessageCount(ClientContext& context) {

}
