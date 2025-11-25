#pragma once

#include <connect_processing/user_actions/userActionBase.h>
#include "session/sessionManager.h"

#include <memory>

class ChatMembersTable;

class UserTyping : UserActionBase {
private:
	SessionManager* _SessionManager;

	std::shared_ptr<ChatMembersTable> _ChatMembersTable;

	const std::string MODULE_NAME_ = "(UserTyping)";
public:
	UserTyping();
	~UserTyping() = default;

	void processing(ClientContext& context) override;
	void notifyChatMembers(uint64_t& UID, uint64_t& chat_id, bool& is_typing);
};