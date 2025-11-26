#pragma once

#include <connect_processing/user_actions/userActionBase.h>
#include "session/sessionManager.h"

#include <memory>
#include <string>

class ChatMembersTable;
class MessageStatusesTable;
class MessagesTable;
class MessageEditsTable;

class UserEditMessage : UserActionBase {
private:
	SessionManager* _SessionManager;

	std::shared_ptr<ChatMembersTable> _ChatMembersTable;
	std::shared_ptr<MessageStatusesTable> _MessageStatuses;
	std::shared_ptr<MessagesTable> _MessagesTable;
	std::shared_ptr<MessageEditsTable> _MessageEditsTable;

	//bool checkActionAvailable();
	//bool checkUserBlock();
	//bool checkInfoCorrectness();
	void notifyChatMembers(uint64_t& UID, uint64_t& chat_id, uint64_t& message_id, std::string& content, std::string& content_type);
	void updateTable(uint64_t& chat_id, uint64_t& message_id, std::string& new_content, uint64_t& editor_uid);

	const std::string MODULE_NAME_ = "(UserEditMessage)";
public:
	UserEditMessage();
	~UserEditMessage() = default;

	void processing(ClientContext& context) override;
};