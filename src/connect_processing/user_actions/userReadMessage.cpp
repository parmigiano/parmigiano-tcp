#include "connect_processing/user_actions/userReadMessage.h"

#include <database/tables/chatMembersTable.h>
//#include <database/tables/messagesTable.h>
#include "database/tables/messageStatusesTable.h"

UserReadMessage::UserReadMessage() {
    _SessionManager = SessionManager::get_instance();

    _ChatMembersTable = std::make_shared<ChatMembersTable>();
    //_MessagesTable = std::make_shared<MessagesTable>();
    _MessageStatuses = std::make_shared<MessageStatusesTable>();
}

void UserReadMessage::processing(ClientContext& context) {
    try {
        ClientRequestStruct::Request request = context.request;

        uint64_t message_id = request.mutable_clientreadmessagepacket()->message_id();
        uint64_t chat_id = request.mutable_clientreadmessagepacket()->chat_id();
        uint64_t UID = request.mutable_clientreadmessagepacket()->uid();

        std::vector<uint64_t> chat_members = _ChatMembersTable->getListOfChatMembers(UID, chat_id);

        updateTable(message_id, chat_members);
        notifyChatMembers(chat_id, message_id, chat_members);
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void UserReadMessage::notifyChatMembers(uint64_t& chat_id, uint64_t& message_id, std::vector<uint64_t> chat_members) {
    for (uint64_t& member_uid : chat_members) {
        boost::asio::ip::tcp::socket& socket = _SessionManager->getSessionSocket(member_uid);

        _SendResponse->setResponseType(read_message);
        _SendResponse->setReadMessageInfo(message_id, chat_id);

        _SendResponse->sendResponse(socket);
    }
}

void UserReadMessage::updateTable(uint64_t& message_id, std::vector<uint64_t> chat_members) {
    for (uint64_t& member_uid : chat_members) {
        _MessageStatuses->updateReadAt(message_id, member_uid);
    }
}
