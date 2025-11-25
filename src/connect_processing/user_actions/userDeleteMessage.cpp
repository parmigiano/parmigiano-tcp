#include "connect_processing/user_actions/userDeleteMessage.h"

#include <database/tables/chatMembersTable.h>
#include <database/tables/messagesTable.h>
#include "database/tables/messageStatusesTable.h"

UserDeleteMessage::UserDeleteMessage() {
    _SessionManager = SessionManager::get_instance();

    _ChatMembersTable = std::make_shared<ChatMembersTable>();
    _MessageStatuses = std::make_shared<MessageStatusesTable>();
    _MessagesTable = std::make_shared<MessagesTable>();
}

void UserDeleteMessage::processing(ClientContext& context) {
    uint64_t UID;

    try {
        ClientRequestStruct::Request request = context.request;

        UID = request.mutable_clientdeletemessagepacket()->uid();
        uint64_t chat_id = request.mutable_clientdeletemessagepacket()->chat_id();
        uint64_t message_id = request.mutable_clientdeletemessagepacket()->message_id();

        _Logger->addSessionLog(_Logger->info, UID, "Delete message (id): " + std::to_string(message_id) + " In chat (id): " + std::to_string(chat_id), 0);

        notifyChatMembers(UID, chat_id, message_id);
        updateTable(chat_id, message_id);
    }
    catch (const std::exception& error) {
        _Logger->addSessionLog(_Logger->warn, UID, MODULE_NAME_ + " except: " + std::string(error.what()), 0);
    }
    catch (...) {
        _Logger->addSessionLog(_Logger->warn, UID, MODULE_NAME_ + " catch unknw error", 0);
    }
}

void UserDeleteMessage::notifyChatMembers(uint64_t& UID, uint64_t& chat_id, uint64_t& message_id) {
    std::vector<uint64_t> chat_members = _ChatMembersTable->getListOfChatMembers(UID, chat_id);

    if (chat_members.empty()) {
        return;
    }

    for (uint64_t& member_uid : chat_members) {
        bool member_online_status = _SessionManager->sessionExist(member_uid);

        if (!member_online_status) {
            continue;
        }

        //_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " notify: " + std::to_string(member_uid), 2);

        Session& session = *_SessionManager->getSessionByUID(member_uid);
        //std::string delivered_at = _MessageStatuses->getDeliveredAt(message_id);

        _SendResponse->setResponseType(delete_message);
        _SendResponse->setDeletedMessageInfo(
            message_id,
            chat_id
        );

        _SendResponse->sendResponse(session);
    }
}

void UserDeleteMessage::updateTable(uint64_t& chat_id, uint64_t& message_id) {
    _MessagesTable->deleteMessage(chat_id, message_id);
}
