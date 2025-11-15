#include "connect_processing/user_actions/userSendMessage.h"

#include <database/tables/chatMembersTable.h>
#include <database/tables/messagesTable.h>
#include "database/tables/messageStatusesTable.h"

#include <vector>

UserSendMessage::UserSendMessage() {
    _SessionManager = SessionManager::get_instance();

    _ChatMembersTable = std::make_shared<ChatMembersTable>();
    _MessagesTable = std::make_shared<MessagesTable>();
    _MessageStatuses = std::make_shared<MessageStatusesTable>();
}

void UserSendMessage::processing(ClientContext& context) {
    try {
        ClientRequestStruct::Request request = context.request;

        uint64_t UID = request.mutable_clientsendmessagepacket()->uid();
        uint64_t chat_id = request.mutable_clientsendmessagepacket()->chat_id();
        std::string content = request.mutable_clientsendmessagepacket()->content();
        std::string content_type = request.mutable_clientsendmessagepacket()->content_type();

        uint64_t message_id = _MessagesTable->addMessage(UID, chat_id, content, content_type);

        sendMessageInterlocutors(message_id, chat_id, UID, content, content_type);
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void UserSendMessage::sendMessageInterlocutors(uint64_t& message_id, uint64_t& chat_id, uint64_t& UID, std::string& content, std::string& content_type) {
    std::vector<uint64_t> chat_members = _ChatMembersTable->getListOfChatMembers(UID, chat_id);

    for (uint64_t& member_uid : chat_members) {
        std::string delivered_at = _MessageStatuses->addMessage(message_id, member_uid);
        boost::asio::ip::tcp::socket& socket = _SessionManager->getSessionSocket(member_uid);

        _SendResponse->setResponseType(send_message);
        _SendResponse->setSendMessageInfo(
            message_id,
            chat_id,
            UID,
            content,
            content_type,
            delivered_at
        );

        _SendResponse->sendResponse(socket);
    }
}
