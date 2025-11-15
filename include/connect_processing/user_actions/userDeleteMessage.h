#pragma once

#include <connect_processing/user_actions/userActionBase.h>

#include <memory>

class UserDeleteMessage : UserActionBase {
private:
public:
	UserDeleteMessage();
	~UserDeleteMessage() = default;

	void processing(ClientContext& context) override;
};