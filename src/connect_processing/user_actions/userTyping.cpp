#include "connect_processing/user_actions/userTyping.h"

#include <database/tables/chatMembersTable.h>

UserTyping::UserTyping() {
    _SessionManager = SessionManager::get_instance();

    _ChatMembersTable = std::make_shared<ChatMembersTable>();
}

void UserTyping::processing(ClientContext& context) {
    uint64_t UID;

    try {
        ClientRequestStruct::Request request = context.request;

        UID = request.mutable_clienttypingpacket()->uid();
        uint64_t chat_id = request.mutable_clienttypingpacket()->chat_id();
        bool is_typing = request.mutable_clienttypingpacket()->is_typing();

        if (is_typing) {
            _Logger->addSessionLog(_Logger->info, UID, "Start typing in chat (id): " + std::to_string(chat_id), 0);
        }
        else {
            _Logger->addSessionLog(_Logger->info, UID, "Finish typing in chat (id):" + std::to_string(chat_id), 0);
        }

        notifyChatMembers(UID, chat_id, is_typing);
    }
    catch (const std::exception& error) {
        //_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
        _Logger->addSessionLog(_Logger->warn, UID, MODULE_NAME_ + " except: " + std::string(error.what()), 0);
    }
    catch (...) {
        _Logger->addSessionLog(_Logger->warn, UID, MODULE_NAME_ + " catch unknw error", 0);
        //_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void UserTyping::notifyChatMembers(uint64_t& UID, uint64_t& chat_id, bool& is_typing) {
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

        _SendResponse->setResponseType(user_typing);
        _SendResponse->setTypingInfo(
            UID,
            chat_id,
            is_typing
        );

        _SendResponse->sendResponse(session);
    }
}
