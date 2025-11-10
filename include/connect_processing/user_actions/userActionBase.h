#pragma once

#include <session/sessionManager.h>

#include <memory>

//class ChatMembersTable;
//class MessagesTable;
//class UserActivesTable;
class SendResponse;

class UserActionBase {
protected:
	SessionManager* _SessionManager;

	std::shared_ptr<SendResponse> _SendResponse;
	//std::shared_ptr<ChatMembersTable> _ChatMembersTable;
	//std::shared_ptr<MessagesTable> _MessagesTable;
	//std::shared_ptr<UserActivesTable> _UserActivesTable;
public:
	UserActionBase();
	virtual ~UserActionBase() = default;
};