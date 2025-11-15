#include "connect_processing/requestProcessing.h"

#include "connect_processing/sendResponse.h"
#include "connect_processing/userStatusService.h"
#include "connect_processing/clientInfoCheck.h"

#include "connect_processing/user_actions/userDeleteMessage.h"
#include "connect_processing/user_actions/userEditMessage.h"
#include "connect_processing/user_actions/userPinMessage.h"
#include "connect_processing/user_actions/userReadMessage.h"
#include "connect_processing/user_actions/userSendMessage.h"
#include "connect_processing/user_actions/userTyping.h"

RequestProcessing::RequestProcessing() {
    _Logger = Logger::get_instance();
    _Config = Config::get_instance();
    _SessionManager = SessionManager::get_instance();
    
    _SendResponse = std::make_shared<SendResponse>();
    _UserStatusService = std::make_shared<UserStatusService>();
    _ClientInfoCheck = std::make_shared<ClientInfoCheck>();

    _UserDeleteMessage = std::make_shared<UserDeleteMessage>();
    _UserEditMessage = std::make_shared<UserEditMessage>();
    _UserPinMessage = std::make_shared<UserPinMessage>();
    _UserReadMessage = std::make_shared<UserReadMessage>();
    _UserSendMessage = std::make_shared<UserSendMessage>();
    _UserTyping = std::make_shared<UserTyping>();

    initDistMap();
    initNamesMap();
}

void RequestProcessing::requestDistribution(std::string request_str, boost::asio::ip::tcp::socket& client_socket) {
    try { 
        ClientRequestStruct::Request accepted_request;
        accepted_request.ParseFromString(request_str);

        if (!_ClientInfoCheck->checkInfoFullness(accepted_request, client_socket)) {
            _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " Not all required data is filled", 2);
            return;
        }

        ClientContext context{ accepted_request, client_socket };
        uint64_t UID = accepted_request.mutable_clientinfo()->uid();

        _SessionManager->assigningSession(client_socket, UID);

        auto it = distribution_map_.find(accepted_request.requestinfo().type());
        if (it != distribution_map_.end()) {
            it->second(context);
            _Logger->addSessionLog(_Logger->info, UID, "new request: " + request_types_name_map_[accepted_request.requestinfo().type()], 2);
        }
        else {
            _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " unkwn requst type", 2);
        }
    } 
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void RequestProcessing::initDistMap() {
    distribution_map_ = {
        {ClientRequestStruct::RequestInfo::send_message, [this](ClientContext& context) { // send_message
            _UserSendMessage->processing(context);
        }},

        {ClientRequestStruct::RequestInfo::read_message, [this](ClientContext& context) { // read_message
            _UserReadMessage->processing(context);
        }},

        {ClientRequestStruct::RequestInfo::edit_message, [this](ClientContext& context) { // edit_message
            _UserEditMessage->processing(context);
        }},

        {ClientRequestStruct::RequestInfo::pin_message, [this](ClientContext& context) { // pin_message
            _UserPinMessage->processing(context);
        }},

        {ClientRequestStruct::RequestInfo::delete_message, [this](ClientContext& context) { // delete_message
           _UserDeleteMessage->processing(context);
        }},

        {ClientRequestStruct::RequestInfo::user_typing, [this](ClientContext& context) { // user_typing
            _UserTyping->processing(context);
        }},

        {ClientRequestStruct::RequestInfo::user_online_status, [this](ClientContext& context) { // user_online_status
            handleUserOnlineStatus(context);
        }},

        {ClientRequestStruct::RequestInfo::get_unread_message_count, [this](ClientContext& context) { // get_unread_message_count
            //_UserSendMessage->processing(ctx);
        }},

        {ClientRequestStruct::RequestInfo::welcome_package, [this](ClientContext& context) { // welcome_package
            handleWelcomePacket(context);
        }},
    };
}

void RequestProcessing::initNamesMap() {
    request_types_name_map_ = {
        {ClientRequestStruct::RequestInfo::send_message, "send_message"},
        {ClientRequestStruct::RequestInfo::read_message, "read_message"},
        {ClientRequestStruct::RequestInfo::edit_message, "edit_message"},
        {ClientRequestStruct::RequestInfo::pin_message, "pin_message"},
        {ClientRequestStruct::RequestInfo::delete_message, "delete_message"},
        {ClientRequestStruct::RequestInfo::user_typing, "user_typing"},
        {ClientRequestStruct::RequestInfo::user_online_status, "user_online_status"},
        {ClientRequestStruct::RequestInfo::get_unread_message_count, "get_unread_message_count"},
        {ClientRequestStruct::RequestInfo::welcome_package, "welcome_package"},
    };
}

void RequestProcessing::handleUserOnlineStatus(ClientContext& context) {
    ClientRequestStruct::Request accepted_request = context.request;
    bool status = accepted_request.mutable_clientactivepacket()->online();
    uint64_t UID = static_cast<uint64_t>(accepted_request.mutable_clientactivepacket()->uid());

    if (status == true) { // user is online now
        _UserStatusService->notifyOffline(UID);
    }
    else if (status == false) { // user is offline :((
        _UserStatusService->notifyOnline(UID);
    }
}

void RequestProcessing::handleWelcomePacket(ClientContext& context) {
    ClientRequestStruct::Request accepted_request = context.request;
    uint64_t UID = static_cast<uint64_t>(accepted_request.mutable_clientactivepacket()->uid());

    _UserStatusService->notifyOnline(UID);
}
