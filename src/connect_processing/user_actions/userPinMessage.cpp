#include "connect_processing/user_actions/userPinMessage.h"

#include <database/tables/chatMembersTable.h>
#include <database/tables/messagesTable.h>
#include <session/session.h>
//#include "database/tables/messageStatusesTable.h"

UserPinMessage::UserPinMessage() {
    _SessionManager = SessionManager::get_instance();

    _ChatMembersTable = std::make_shared<ChatMembersTable>();
    _MessagesTable = std::make_shared<MessagesTable>();
    //_MessageStatuses = std::make_shared<MessageStatusesTable>();
}

void UserPinMessage::processing(ClientContext& context) {
    uint64_t UID;

    try {
        ClientRequestStruct::Request request = context.request;

        UID = request.mutable_clientpinmessagepacket()->uid();
        uint64_t chat_id = request.mutable_clientpinmessagepacket()->chat_id();
        uint64_t message_id = request.mutable_clientpinmessagepacket()->message_id();
        bool pin_status = _MessagesTable->getPinStatus(message_id) ? false : true;

        if (pin_status) {
            _Logger->addSessionLog(_Logger->info, UID, "Pin message (id): " + std::to_string(message_id) + " In chat (id): " + std::to_string(chat_id), 0);
        }
        else {
            _Logger->addSessionLog(_Logger->info, UID, "Unpin message (id): " + std::to_string(message_id) + " In chat (id): " + std::to_string(chat_id), 0);
        }

        notifyChatMembers(UID, chat_id, message_id, pin_status);
        updateTable(chat_id, message_id, pin_status);
    }
    catch (const std::exception& error) {
        _Logger->addSessionLog(_Logger->warn, UID, MODULE_NAME_ + " except: " + std::string(error.what()), 0);
    }
    catch (...) {
        _Logger->addSessionLog(_Logger->warn, UID, MODULE_NAME_ + " catch unknw error", 0);
    }
}

void UserPinMessage::notifyChatMembers(uint64_t& UID, uint64_t& chat_id, uint64_t& message_id, bool& pin_status) {
    std::vector<uint64_t> chat_members = _ChatMembersTable->getListOfChatMembers(UID, chat_id);

    if (chat_members.empty()) {
        return;
    }

    for (uint64_t & member_uid : chat_members) {
        bool member_online_status = _SessionManager->sessionExist(member_uid);

        if (!member_online_status) {
            continue;
        }

        Session& session = *_SessionManager->getSessionByUID(member_uid);

        _SendResponse->setResponseType(send_message);
        _SendResponse->setPinnedMessageInfo(UID, chat_id, message_id, pin_status);

        _SendResponse->sendResponse(session);
    }
}

void UserPinMessage::updateTable(uint64_t& chat_id, uint64_t& message_id, bool& pin_status) {
    _MessagesTable->updateIsPinned(chat_id, message_id, pin_status);
}
