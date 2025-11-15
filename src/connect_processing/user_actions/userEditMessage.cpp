#include "connect_processing/user_actions/userEditMessage.h"

#include <database/tables/chatMembersTable.h>
#include <database/tables/messagesTable.h>
#include "database/tables/messageStatusesTable.h"

UserEditMessage::UserEditMessage() {
    _SessionManager = SessionManager::get_instance();

    _ChatMembersTable = std::make_shared<ChatMembersTable>();
    _MessageStatuses = std::make_shared<MessageStatusesTable>();
    _MessagesTable = std::make_shared<MessagesTable>();
}

void UserEditMessage::processing(ClientContext& context) {
    try {
        ClientRequestStruct::Request request = context.request;

        uint64_t UID = request.mutable_clienteditmessagepacket()->uid();
        uint64_t chat_id = request.mutable_clienteditmessagepacket()->chat_id();
        uint64_t message_id = request.mutable_clienteditmessagepacket()->message_id();
        std::string content = request.mutable_clienteditmessagepacket()->content();
        std::string content_type = request.mutable_clienteditmessagepacket()->content_type();

        updateTable(chat_id, message_id);
        notifyChatMembers(UID, chat_id, message_id, content, content_type);
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void UserEditMessage::notifyChatMembers(uint64_t& UID, uint64_t& chat_id, uint64_t& message_id, std::string& content, std::string& content_type) {
    std::vector<uint64_t> chat_members = _ChatMembersTable->getListOfChatMembers(UID, chat_id);
    //std::map <std::string, std::string> message_info_map = _MessagesTable->getMessageInfo(message_id);

    for (uint64_t& member_uid : chat_members) {
        boost::asio::ip::tcp::socket& socket = _SessionManager->getSessionSocket(member_uid);
        std::string delivered_at = _MessageStatuses->getDeliveredAt(message_id);

        _SendResponse->setResponseType(pin_message);
        _SendResponse->setEditedMessageInfo(
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

void UserEditMessage::updateTable(uint64_t& chat_id, uint64_t& message_id) {
    _MessagesTable->updateIsEdited(chat_id, message_id);
}
