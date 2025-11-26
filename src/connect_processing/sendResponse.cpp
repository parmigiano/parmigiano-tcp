#include "connect_processing/sendResponse.h"

#include <regex>
#include <filesystem>
#include <session/session.h>

SendResponse::SendResponse(){
    _Logger = Logger::get_instance();
    _Config = Config::get_instance();
    
    _Response = std::make_shared<ResponseStruct::Response>();

    initTypesArrays();
}

void SendResponse::initTypesArrays() {
    response_types_arr_ = {
        ResponseStruct::ResponseInfo::send_message,
        ResponseStruct::ResponseInfo::read_message,
        ResponseStruct::ResponseInfo::edit_message,
        ResponseStruct::ResponseInfo::pin_message,
        ResponseStruct::ResponseInfo::delete_message,
        ResponseStruct::ResponseInfo::user_typing,
        ResponseStruct::ResponseInfo::user_online_status,
        ResponseStruct::ResponseInfo::get_unread_message_count
    };

    disconnect_types_arr_ = {
        ResponseStruct::DisconnectNotifying_types_warn,
        ResponseStruct::DisconnectNotifying_types_error,
        ResponseStruct::DisconnectNotifying_types_tempBan,
        ResponseStruct::DisconnectNotifying_types_inactive,
        ResponseStruct::DisconnectNotifying_types_littleInfo
    };
}


void SendResponse::setResponseType(responseType response_type) {
    try {
        auto* response_info = _Response->mutable_responseinfo();

        if (response_type >= 0 && response_type < RESPONSE_TYPE_COUNT) {
            response_info->set_type(response_types_arr_[response_type]);
        } 
        else {
            throw std::runtime_error("unknw response_type");
        }
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void SendResponse::setDisonnectType(disconnectType disconnect_type) {
    try {
        auto* disconnect_info = _Response->mutable_disconnectnotifying();

        if (disconnect_type >= 0 && disconnect_type < DISCONNECT_TYPE_COUNT) {
            disconnect_info->set_type(disconnect_types_arr_[disconnect_type]);
        }
        else {
            throw std::runtime_error("unknw disconnect_type");
        }
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void SendResponse::setDisconnectInfo(std::string& description, short int& code) {
        try {
            auto* disconnect_info = _Response->mutable_disconnectnotifying();
    
            disconnect_info->set_description(description);
            disconnect_info->set_code(code);
        }
        catch (const std::exception& error) {
            _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
        }
        catch (...) {
            _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
        }
}

void SendResponse::setClientActiveInfo(uint64_t& UID, bool status) {
    try {
        auto* client_active_info = _Response->mutable_clientactive();

        client_active_info->set_uid(UID);
        client_active_info->set_online(status);
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void SendResponse::setSendMessageInfo(uint64_t& message_id, uint64_t& temp_message_id, uint64_t& chat_id, uint64_t& sender_UID, std::string& content, std::string& content_type, std::string& delivered_at) {
    try {
        auto* message_info = _Response->mutable_clientsendmessage();

        message_info->set_message_id(message_id);
        message_info->set_temp_message_id(temp_message_id);
        message_info->set_chat_id(chat_id);
        message_info->set_sender_uid(sender_UID);
        message_info->set_content(content);
        message_info->set_content_type(content_type);
        message_info->set_delivered_at(delivered_at);
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void SendResponse::clearResponse(){
    _Response->Clear();
}

void SendResponse::setReadMessageInfo(uint64_t& message_id, uint64_t& chat_id) {
    try {
        auto* read_message_info = _Response->mutable_clientreadmessage();
        
        read_message_info->set_chat_id(chat_id);
        read_message_info->set_message_id(message_id);
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void SendResponse::setPinnedMessageInfo(uint64_t& UID, uint64_t& chat_id, uint64_t& message_id, bool& pin_status) {
    try {
        auto* pinned_message_info = _Response->mutable_clientpinmessage();

        pinned_message_info->set_uid(UID);
        pinned_message_info->set_chat_id(chat_id);
        pinned_message_info->set_message_id(message_id);
        pinned_message_info->set_pin_status(pin_status);
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void SendResponse::setEditedMessageInfo(uint64_t& message_id, uint64_t& chat_id, uint64_t& sender_UID, std::string& content, std::string& content_type, std::string& delivered_at) {
    try {
        auto* edited_message_info = _Response->mutable_clienteditmessage();

        edited_message_info->set_message_id(message_id);
        edited_message_info->set_chat_id(chat_id);
        edited_message_info->set_sender_uid(sender_UID);
        edited_message_info->set_content(content);
        edited_message_info->set_content_type(content_type);
        edited_message_info->set_delivered_at(delivered_at);
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void SendResponse::setDeletedMessageInfo(uint64_t& message_id, uint64_t& chat_id) {
    try {
        auto* deleted_message_info = _Response->mutable_clientdeletemessage();

        deleted_message_info->set_message_id(message_id);
        deleted_message_info->set_chat_id(chat_id);
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void SendResponse::setTypingInfo(uint64_t& UID, uint64_t& chat_id, bool& is_typing) {
    try {
        auto* client_typing_info = _Response->mutable_clienttyping();


        client_typing_info->set_uid(UID);
        client_typing_info->set_chat_id(chat_id);
        client_typing_info->set_is_typing(is_typing);
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

std::ifstream SendResponse::openFile(std::string& filepath, unsigned int& file_size) {
    std::ifstream file(filepath, std::ios::in | std::ios::binary);

    if (!file) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    if (file_size == 0) {
        throw std::runtime_error("empty file " + filepath);
    }

    return file;
}

void SendResponse::sendResponse(Session& session) {
    try {
        std::string data;

        if (!_Response->SerializeToString(&data)) {
            throw std::runtime_error("Failed to serialize request");
            //_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " Failed to serialize request", 2);
        }

        if (_Response->responseinfo().type() == ResponseStruct::ResponseInfo::unknown) {
            //_Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " Request will not sending with type requestInfo->requestType: unknown", 2);
            throw std::runtime_error("Request will not sending with type requestInfo->requestType: unknown");
        }

        uint32_t size = static_cast<uint32_t>(data.size());
        //uint32_t size_net = size;

        auto send_buffer = std::make_shared<std::string>();
        send_buffer->resize(4 + data.size());

        std::memcpy(send_buffer->data(), &size, 4);
        std::memcpy(send_buffer->data() + 4, data.data(), data.size()); // preparing header + body

        //auto self = shared_from_this();

        boost::asio::ip::tcp::socket* client_socket = &session.socket();

        client_socket->async_send(boost::asio::buffer(*send_buffer), 0, [send_buffer, &session] (const boost::system::error_code& error, std::size_t bytes) {
            //_Config->write_handler_ptr(error, bytes);
            session.handleWrite(error, bytes);
        });

        data.clear();
        clearResponse();
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void SendResponse::sendFile(std::string& filepath, boost::asio::ip::tcp::socket& client_socket) {
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    filepath = _Config->configuration_vars_["buildDir"] + "/" + filepath;
    unsigned int file_size = std::filesystem::file_size(filepath);

    try {
        std::ifstream file = openFile(filepath, file_size);

        for (int i = 0; i < file_size / BUFFER_SIZE; i++) {
            memset(buffer, 0, sizeof(buffer));

            file.seekg(sizeof(buffer) * i);

            if (file_size < BUFFER_SIZE) {
                file.read(buffer, file_size);
                client_socket.async_send(boost::asio::buffer(buffer, file_size), 0, [this](const boost::system::error_code& error, std::size_t bytes) {
                    _Config->write_handler_ptr(error, bytes); 
                });
            }
            else {
                file.read(buffer, sizeof(buffer));
                client_socket.async_send(boost::asio::buffer(buffer, BUFFER_SIZE), 0, [this](const boost::system::error_code& error, std::size_t bytes) {
                    _Config->write_handler_ptr(error, bytes); 
                });
            }
        }

        client_socket.async_send(boost::asio::buffer("end", 3), 0, [this](const boost::system::error_code& error, std::size_t bytes) {
            _Config->write_handler_ptr(error, bytes); 
        });

        file.close(); 
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
        client_socket.async_send(boost::asio::buffer("end", 3), 0, [this](const boost::system::error_code& error, std::size_t bytes) {
            _Config->write_handler_ptr(error, bytes); 
        });
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
        client_socket.async_send(boost::asio::buffer("end", 3), 0, [this](const boost::system::error_code& error, std::size_t bytes) {
            _Config->write_handler_ptr(error, bytes); 
        });
    }
}
