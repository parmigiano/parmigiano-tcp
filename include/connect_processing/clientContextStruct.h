#pragma once

#include "ClientRequestStruct.pb.h"
#include <session/session.h>

struct ClientContext {
	Session& session;
	ClientRequestStruct::Request& request;
};