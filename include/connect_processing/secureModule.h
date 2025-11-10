#pragma once

#include "logger.h"
#include "config.h"

#include <memory>

class SendResponse;

class SecureModule {
private:
	Config* _Config;
	Logger* _Logger;

	std::shared_ptr<SendResponse> _SendResponse;
public:
	SecureModule();
	~SecureModule() = default;
};