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
    uint64_t UID;

    try {
        ClientRequestStruct::Request request = context.request;

        UID = request.mutable_clienteditmessagepacket()->uid();
        uint64_t chat_id = request.mutable_clienteditmessagepacket()->chat_id();
        uint64_t message_id = request.mutable_clienteditmessagepacket()->message_id();
        std::string content = request.mutable_clienteditmessagepacket()->content();
        std::string content_type = request.mutable_clienteditmessagepacket()->content_type();

        _Logger->addSessionLog(_Logger->info, UID, "Edit message (id): " + std::to_string(message_id) + " In chat (id): " + std::to_string(chat_id) + " To message (size): " + std::to_string(content.size()), 0);

        /*if (chat_id <= 0 || message_id <= 0) {
            _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " unknw chat_id or message_id", 2);
            return;
        }*/

        notifyChatMembers(UID, chat_id, message_id, content, content_type);
        updateTable(chat_id, message_id);
    }
    catch (const std::exception& error) {
        _Logger->addSessionLog(_Logger->warn, UID, MODULE_NAME_ + " except: " + std::string(error.what()), 0);
    }
    catch (...) {
        _Logger->addSessionLog(_Logger->warn, UID, MODULE_NAME_ + " catch unknw error", 0);
    }
}

void UserEditMessage::notifyChatMembers(uint64_t& UID, uint64_t& chat_id, uint64_t& message_id, std::string& content, std::string& content_type) {
    std::vector<uint64_t> chat_members = _ChatMembersTable->getListOfChatMembers(UID, chat_id);
    //std::map <std::string, std::string> message_info_map = _MessagesTable->getMessageInfo(message_id);

    if (chat_members.empty()) {
        return;
    }

    for (uint64_t& member_uid : chat_members) {
        bool member_online_status = _SessionManager->sessionExist(member_uid);

        if (!member_online_status) {
            continue;
        }

        Session& session = *_SessionManager->getSessionByUID(member_uid);

        /*std::shared_ptr<boost::asio::ip::tcp::socket> socket = std::make_shared<boost::asio::ip::tcp::socket>(std::move(_SessionManager->getSessionSocket(member_uid)));

        if (!socket->is_open()) {
            continue;
        }*/

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

        _SendResponse->sendResponse(session);
    }
}

void UserEditMessage::updateTable(uint64_t& chat_id, uint64_t& message_id) {
    _MessagesTable->updateIsEdited(chat_id, message_id);
}
