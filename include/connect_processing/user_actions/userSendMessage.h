#pragma once

#include <connect_processing/user_actions/userActionBase.h>
#include "session/sessionManager.h"

#include <memory>
#include <string>

class ChatMembersTable;
class MessagesTable;
class MessageStatusesTable;

class UserSendMessage : UserActionBase {
private:
	SessionManager* _SessionManager;

	std::shared_ptr<ChatMembersTable> _ChatMembersTable;
	std::shared_ptr<MessagesTable> _MessagesTable;
	std::shared_ptr<MessageStatusesTable> _MessageStatuses;

	//bool checkActionAvailable();
	//bool checkUserBlock();
	//bool checkInfoCorrectness();
	void sendToSender(uint64_t& message_id, uint64_t& temp_message_id, uint64_t& chat_id, uint64_t& UID, std::string& content, std::string& content_type);
	void sendMessageToChatMembers(uint64_t& message_id, uint64_t& chat_id, uint64_t& UID, std::string& content, std::string& content_type);
	uint64_t addToTable(uint64_t& UID, uint64_t& chat_id, std::string& content, std::string& content_type);

	const std::string MODULE_NAME_ = "(UserSendMessage)";
public:
	UserSendMessage();
	~UserSendMessage() = default;

	void processing(ClientContext& context) override;
};