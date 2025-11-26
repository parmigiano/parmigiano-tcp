#pragma once

#include <connect_processing/user_actions/userActionBase.h>
#include "session/sessionManager.h"

#include <memory>

class ChatMembersTable;
class MessagesTable;
//class MessageStatusesTable;

class UserPinMessage : UserActionBase {
private:
	SessionManager* _SessionManager;

	std::shared_ptr<ChatMembersTable> _ChatMembersTable;
	std::shared_ptr<MessagesTable> _MessagesTable;
	//std::shared_ptr<MessageStatusesTable> _MessageStatuses;

	//bool checkActionAvailable();
	//bool checkUserBlock();
	//bool checkInfoCorrectness();
	void notifyChatMembers(uint64_t& UID, uint64_t& chat_id, uint64_t& message_id, bool& pin_status);
	void updateTable(uint64_t& chat_id, uint64_t& message_id, bool& pin_status);

	const std::string MODULE_NAME_ = "(UserPinMessage)";
public:
	UserPinMessage();
	~UserPinMessage() = default;

	void processing(ClientContext& context) override;
};