#pragma once

#include <connect_processing/user_actions/userActionBase.h>

#include <memory>

class ChatMembersTable;
class MessageStatusesTable;
class MessagesTable;

class UserDeleteMessage : UserActionBase {
private:
	SessionManager* _SessionManager;

	std::shared_ptr<ChatMembersTable> _ChatMembersTable;
	std::shared_ptr<MessageStatusesTable> _MessageStatuses;
	std::shared_ptr<MessagesTable> _MessagesTable;

	//bool checkActionAvailable();
	//bool checkUserBlock();
	//bool checkInfoCorrectness();
	void notifyChatMembers(uint64_t& UID, uint64_t& chat_id, uint64_t& message_id);
	void updateTable(uint64_t& chat_id, uint64_t& message_id);

	const std::string MODULE_NAME_ = "(UserDeleteMessage)";
public:
	UserDeleteMessage();
	~UserDeleteMessage() = default;

	void processing(ClientContext& context) override;
};