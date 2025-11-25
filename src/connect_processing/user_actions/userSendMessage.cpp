#include "connect_processing/user_actions/userSendMessage.h"

#include <database/tables/chatMembersTable.h>
#include <database/tables/messagesTable.h>
#include "database/tables/messageStatusesTable.h"
#include "session/session.h"

#include <vector>

UserSendMessage::UserSendMessage() {
    _SessionManager = SessionManager::get_instance();

    _ChatMembersTable = std::make_shared<ChatMembersTable>();
    _MessagesTable = std::make_shared<MessagesTable>();
    _MessageStatuses = std::make_shared<MessageStatusesTable>();
}

void UserSendMessage::processing(ClientContext& context) {
    uint64_t UID;

    try {
        ClientRequestStruct::Request request = context.request;

        UID = request.mutable_clientsendmessagepacket()->uid();
        uint64_t chat_id = request.mutable_clientsendmessagepacket()->chat_id();
        std::string content = request.mutable_clientsendmessagepacket()->content();
        std::string content_type = request.mutable_clientsendmessagepacket()->content_type();

        uint64_t message_id = _MessagesTable->addMessage(UID, chat_id, content, content_type);

        _Logger->addSessionLog(_Logger->info, UID, "Send message (size): " + std::to_string(content.size()) + " Message (id): " + std::to_string(message_id) + " To chat (id): " + std::to_string(chat_id), 0);

        sendMessageToChatMembers(message_id, chat_id, UID, content, content_type);
    }
    catch (const std::exception& error) {
        _Logger->addSessionLog(_Logger->warn, UID, MODULE_NAME_ + " except: " + std::string(error.what()), 0);
    }
    catch (...) {
        _Logger->addSessionLog(_Logger->warn, UID, MODULE_NAME_ + " catch unknw error", 0);
    }
}

void UserSendMessage::sendMessageToChatMembers(uint64_t& message_id, uint64_t& chat_id, uint64_t& UID, std::string& content, std::string& content_type) {
    std::vector<uint64_t> chat_members = _ChatMembersTable->getListOfChatMembers(UID, chat_id);

    if (chat_members.empty()) {
        return;
    }

    for (uint64_t& member_uid : chat_members) {
        bool member_online_status = _SessionManager->sessionExist(member_uid);
        std::string delivered_at = _MessageStatuses->addMessage(message_id, member_uid);

        if (!member_online_status) {
            continue;
        }

        Session& session = *_SessionManager->getSessionByUID(member_uid);

        _SendResponse->setResponseType(send_message);
        _SendResponse->setSendMessageInfo(
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
