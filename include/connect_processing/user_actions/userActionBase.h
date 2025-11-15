#pragma once

#include "connect_processing/clientContextStruct.h"
#include "connect_processing/responseTypeEnum.h"
#include <session/sessionManager.h>
#include <logger.h>
#include "ClientRequestStruct.pb.h"
#include "connect_processing/sendResponse.h"

#include <memory>

//class SendResponse;

class UserActionBase {
protected:
	SessionManager* _SessionManager;
	Logger* _Logger;

	std::shared_ptr<SendResponse> _SendResponse;
public:
	UserActionBase();
	virtual ~UserActionBase() = default;

	virtual void processing(ClientContext& ctx) = 0;
};