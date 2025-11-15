#pragma once

#include "logger.h"
#include "config.h"

#include "ClientRequestStruct.pb.h"
#include "ResponseStruct.pb.h"
#include "connect_processing/responseTypeEnum.h"
#include "connect_processing/disconnectTypeEnum.h"

#include <string>
#include <fstream>
#include <memory>
#include <boost/asio.hpp>

//class Session;

class SendResponse {
private:
	Logger* _Logger;
	Config* _Config;

	std::shared_ptr<ResponseStruct::Response> _Response;

	void clearResponse();

	std::array<ResponseStruct::ResponseInfo_types, 8> response_types_arr_;
	std::array<ResponseStruct::DisconnectNotifying_types, 5> disconnect_types_arr_;

	void initTypesArrays();

	const std::string MODULE_NAME_ = "(SendResponse)";
public:
	SendResponse();
	~SendResponse() = default;

	void setResponseType(responseType response_type);
	void setDisonnectType(disconnectType disconnect_type);

	void setDisconnectInfo(std::string& description, short int& code);
	/*void setDisconnectDescription(std::string& description);
	void setDisconnectCode(short int& code);*/

	void setClientActiveInfo(uint64_t& UID, bool status);
	/*void setClientActiveUID(uint64_t& UID);
	void setClientActiveOnlineStatus(bool status);*/

	void setSendMessageInfo(uint64_t& message_id, uint64_t& chat_id, uint64_t& sender_UID, std::string& content, std::string& content_type, std::string& delivered_at);
	/*void setSendMessageID();
	void setSendMessageChatID();
	void setSendMessageSenderUID();
	void setSendMessageContent();
	void setSendMessageContentType();
	void setSendMessageDeliveredAt();*/

	void setReadMessageInfo(uint64_t& message_id, uint64_t& chat_id);

	void setPinnedMessageInfo(uint64_t& message_id, uint64_t& chat_id, uint64_t& sender_UID, std::string& content, std::string& content_type, std::string& delivered_at);

	void setEditedMessageInfo(uint64_t& message_id, uint64_t& chat_id, uint64_t& sender_UID, std::string& content, std::string& content_type, std::string& delivered_at);

	std::ifstream openFile(std::string& filepath, unsigned int& file_size);

	void sendResponse(boost::asio::ip::tcp::socket& client_socket);
	void sendFile(std::string& filepath, boost::asio::ip::tcp::socket& client_socket);
};