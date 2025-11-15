#pragma once

#include "ClientRequestStruct.pb.h"
#include <boost/asio.hpp>

struct ClientContext {
	ClientRequestStruct::Request request;
	boost::asio::ip::tcp::socket& socket;
};