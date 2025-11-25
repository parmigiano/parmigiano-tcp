#pragma once

#include "ClientRequestStruct.pb.h"
#include <boost/asio.hpp>
#include <session/session.h>

struct ClientContext {
	Session& session;
	ClientRequestStruct::Request request;
};