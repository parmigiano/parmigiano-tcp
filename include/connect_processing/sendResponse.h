#pragma once

#include "logger.h"
#include "config.h"

#include "ClientRequestStruct.pb.h"
#include "ResponseStruct.pb.h"

#include <string>
#include <fstream>
#include <memory>
#include <boost/asio.hpp>

//class Session;

class SendResponse {
public:
	enum disconnectType {
		warn,
		error,
		tempBan,
		inactive,
		littleInfo
	};

	enum responseType { 
		file_hash_request, 
		download_file, 
		send_message, 
		read_message, 
		edit_message, 
		pin_message, 
		delete_message, 
		user_typing, 
		user_online_status, 
		get_unread_message_count
	};

private:
	Logger* _Logger;
	Config* _Config;

	std::shared_ptr<ResponseStruct::Response> _Response;

	void clearResponse();

	const std::string MODULE_NAME_ = "(SendResponse)";
public:
	SendResponse();
	~SendResponse() = default;

	void setReponseType(responseType response_type);

	void setDisonnectType(disconnectType disconnect_type);
	void setDisconnectDescription(std::string& description);
	void setDisconnectCode(short int& code);

	void setClientActiveUID(uint64_t& UID);
	void setClientActiveOnlineStatus(bool status);

	std::ifstream openFile(std::string& filepath, unsigned int& file_size);

	void sendResponse(boost::asio::ip::tcp::socket& client_socket);
	void sendFile(std::string& filepath, boost::asio::ip::tcp::socket& client_socket);
};