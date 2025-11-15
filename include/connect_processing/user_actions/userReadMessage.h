#pragma once

#include <connect_processing/user_actions/userActionBase.h>
#include "session/sessionManager.h"

#include <string>
#include <memory>

class ChatMembersTable;
class MessagesTable;
class MessageStatusesTable;

class UserReadMessage : UserActionBase {
private:
	SessionManager* _SessionManager;

	std::shared_ptr<ChatMembersTable> _ChatMembersTable;
	//std::shared_ptr<MessagesTable> _MessagesTable;
	std::shared_ptr<MessageStatusesTable> _MessageStatuses;

	void notifyChatMembers(uint64_t& chat_id, uint64_t& message_id, std::vector<uint64_t> chat_members);
	void updateTable(uint64_t& message_id, std::vector<uint64_t> chat_members);

	const std::string MODULE_NAME_ = "(UserReadMessage)";
public:
	UserReadMessage();
	~UserReadMessage() = default;

	void processing(ClientContext& context) override;
};