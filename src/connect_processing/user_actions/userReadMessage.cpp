#include "connect_processing/user_actions/userReadMessage.h"

#include <database/tables/chatMembersTable.h>
//#include <database/tables/messagesTable.h>
#include "database/tables/messageStatusesTable.h"
#include "session/session.h"

UserReadMessage::UserReadMessage() {
    _SessionManager = SessionManager::get_instance();

    _ChatMembersTable = std::make_shared<ChatMembersTable>();
    //_MessagesTable = std::make_shared<MessagesTable>();
    _MessageStatuses = std::make_shared<MessageStatusesTable>();
}

void UserReadMessage::processing(ClientContext& context) {
    uint64_t UID;

    try {
        ClientRequestStruct::Request request = context.request;

        UID = request.mutable_clientreadmessagepacket()->uid();
        uint64_t chat_id = request.mutable_clientreadmessagepacket()->chat_id();
        uint64_t message_id = request.mutable_clientreadmessagepacket()->message_id();

        _Logger->addSessionLog(_Logger->info, UID, "Read message (id): " + std::to_string(message_id) + " In chat (id): " + std::to_string(chat_id), 0);

        std::vector<uint64_t> chat_members = _ChatMembersTable->getListOfChatMembers(UID, chat_id);

        updateTable(message_id, chat_members);
        notifyChatMembers(chat_id, message_id, chat_members);
    }
    catch (const std::exception& error) {
        _Logger->addSessionLog(_Logger->warn, UID, MODULE_NAME_ + " except: " + std::string(error.what()), 0);
    }
    catch (...) {
        _Logger->addSessionLog(_Logger->warn, UID, MODULE_NAME_ + " catch unknw error", 0);
    }
}

void UserReadMessage::notifyChatMembers(uint64_t& chat_id, uint64_t& message_id, std::vector<uint64_t> chat_members) {
    if (chat_members.empty()) {
        return;
    }

    for (uint64_t& member_uid : chat_members) {
        bool member_online_status = _SessionManager->sessionExist(member_uid);

        if (!member_online_status) {
            continue;
        }

        Session& session = *_SessionManager->getSessionByUID(member_uid);

        _SendResponse->setResponseType(read_message);
        _SendResponse->setReadMessageInfo(message_id, chat_id);

        _SendResponse->sendResponse(session);
    }
}

void UserReadMessage::updateTable(uint64_t& message_id, std::vector<uint64_t> chat_members) {
    for (uint64_t& member_uid : chat_members) {
        _MessageStatuses->updateReadAt(message_id, member_uid);
    }
}
