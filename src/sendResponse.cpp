#include "sendResponse.h"

#include <regex>
#include <filesystem>

SendResponse::SendResponse(){
    _Logger = Logger::get_instance();
    _Config = Config::get_instance();
    
    _Response = std::make_shared<ResponseStruct::Response>();
}

void SendResponse::setReponseType(responseType response_type){
    try {
        auto* response_info = _Response->mutable_disconnectnotifying();

        static const std::unordered_map<responseType, ResponseStruct::ResponseInfo_types> types = {
            { send_message, ResponseStruct::ResponseInfo::send_message },
            { read_message, ResponseStruct::ResponseInfo::read_message },
            { edit_message, ResponseStruct::ResponseInfo::edit_message },
            { pin_message, ResponseStruct::ResponseInfo::pin_message },
            { delete_message, ResponseStruct::ResponseInfo::delete_message },
            { user_typing, ResponseStruct::ResponseInfo::user_typing },
            { user_online_status, ResponseStruct::ResponseInfo::user_online_status },
            { get_unread_message_count, ResponseStruct::ResponseInfo::get_unread_message_count }
        };

        auto it = types.find(response_type);
        if (it != types.end()) {
            _Response->mutable_responseinfo()->set_type(it->second);
        }
        else {
            _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " unknown response type", 2);
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

        static const std::unordered_map<disconnectType, ResponseStruct::DisconnectNotifying_types> types = {
            { warn, ResponseStruct::DisconnectNotifying_types_warn },
            { error, ResponseStruct::DisconnectNotifying_types_error },
            { tempBan, ResponseStruct::DisconnectNotifying_types_tempBan },
            { inactive, ResponseStruct::DisconnectNotifying_types_inactive },
            { littleInfo, ResponseStruct::DisconnectNotifying_types_littleInfo }
        };

        auto it = types.find(disconnect_type);
        if (it != types.end()) {
            disconnect_info->set_type(it->second);
        }
        else {
            _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " unknown response type", 2);
        }
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void SendResponse::setDisconnectDescription(std::string& description) {
    try {
        auto* disconnect_info = _Response->mutable_disconnectnotifying();

        disconnect_info->set_description(description);
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void SendResponse::setDisconnectCode(short int& code) {
    try {
        auto* disconnect_info = _Response->mutable_disconnectnotifying();

        disconnect_info->set_code(code);
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void SendResponse::setClientActiveUID(uint64_t& UID) {
    try {
        auto* client_active = _Response->mutable_clientactivepacket();

        client_active->set_uid(UID);
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void SendResponse::setClientActiveOnlineStatus(bool status) {
    try {
        auto* client_active = _Response->mutable_clientactivepacket();

        client_active->set_online(status);
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

std::ifstream SendResponse::openFile(std::string& filepath, unsigned int& file_size){
    std::ifstream file(filepath, std::ios::in | std::ios::binary);

    if (!file) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    if (file_size == 0) {
        throw std::runtime_error("empty file " + filepath);
    }

    return file;
}

void SendResponse::sendResponse(boost::asio::ip::tcp::socket& client_socket){
    try {
        std::string data;

        if (!_Response->SerializeToString(&data)) {
            _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " Failed to serialize request", 2);
        }

        if (_Response->responseinfo().type() == ResponseStruct::ResponseInfo::unknown) {
            _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " Request will not sending with type requestInfo->requestType: unknown", 2);
        }

        client_socket.async_send(boost::asio::buffer(data.c_str(), (int)data.size()), 0, [this](const boost::system::error_code& error, std::size_t bytes) {
            _Config->write_handler_ptr(error, bytes);
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

void SendResponse::sendFile(std::string& filepath, boost::asio::ip::tcp::socket& client_socket){
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
