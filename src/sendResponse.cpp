#include "../include/sendResponse.h"

#include <regex>
#include <filesystem>

SendResponse::SendResponse(){
    _Logger = Logger::get_instance();
    _Config = Config::get_instance();
    
    _Response = std::make_shared<ResponseStruct::Response>();
}

void SendResponse::setReponseType(std::string responseType){
    if (responseType == "filesHashes") {
        _Response->mutable_responseinfo()->set_type(ResponseStruct::ResponseInfo::fileHashRequest);
    }
    else if (responseType == "downloadFile") {
        _Response->mutable_responseinfo()->set_type(ResponseStruct::ResponseInfo::downloadFile);
    }
    else {
        _Logger->addServerLog(_Logger->warn, "(sendResponse) unknown response type", 2);
    }
}

void SendResponse::setDisonnectType(disconnectType type) {
    auto* disconnectInfo = _Response->mutable_disconnectnotifying();

    switch (type) {
    case SendResponse::warn:
        disconnectInfo->set_type(ResponseStruct::DisconnectNotifying_types_warn);
        break;
    case SendResponse::error:
        disconnectInfo->set_type(ResponseStruct::DisconnectNotifying_types_error);
        break;
    case SendResponse::tempBan:
        disconnectInfo->set_type(ResponseStruct::DisconnectNotifying_types_tempBan);
        break;
    case SendResponse::inactive:
        disconnectInfo->set_type(ResponseStruct::DisconnectNotifying_types_inactive);
        break;
    case SendResponse::unknown:
        disconnectInfo->set_type(ResponseStruct::DisconnectNotifying_types_unknown);
        break;
    }
}

void SendResponse::setDisconnectDescription(std::string description) {
    auto* disconnectInfo = _Response->mutable_disconnectnotifying();

    disconnectInfo->set_description(description);
}

void SendResponse::setDisconnectCode(short int code) {
    auto* disconnectInfo = _Response->mutable_disconnectnotifying();

    disconnectInfo->set_code(code);
}

void SendResponse::clearResponse(){
    _Response->Clear();
}

std::ifstream SendResponse::openFile(std::string filePath, unsigned int fileSize){
    std::ifstream file(filePath, std::ios::in | std::ios::binary);

    if (!file) {
        throw std::runtime_error("(sendResponse) Failed to open file: " + filePath);
    }

    if (fileSize == 0) {
        throw std::runtime_error("(sendResponse) empty file " + filePath);
    }

    return file;
}

int SendResponse::sendResponse(boost::asio::ip::tcp::socket& socket){
    std::string data;

    if (!_Response->SerializeToString(&data)) {
        _Logger->addServerLog(_Logger->warn, "(sendResponse) Failed to serialize request", 2);
        return 1;
    }

    if (_Response->responseinfo().type() == ResponseStruct::ResponseInfo::unknown) {
        _Logger->addServerLog(_Logger->warn, "(sendResponse) Request will not sending with type requestInfo->requestType: unknown", 2);
        return 1;
    }

    socket.async_send(boost::asio::buffer(data.c_str(), (int)data.size()), 0, [this](const boost::system::error_code& error, std::size_t bytes) {
        _Config->write_handler_ptr(error, bytes); 
    });

    data.clear();
    clearResponse();

	return 0;
}

int SendResponse::sendFile(std::string filePath, boost::asio::ip::tcp::socket& socket){
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    filePath = _Config->configurationVars["buildDir"] + "/" + filePath;
    unsigned int fileSize = std::filesystem::file_size(filePath);

    try {
        std::ifstream file = openFile(filePath, fileSize);

        for (int i = 0; i < fileSize / BUFFER_SIZE; i++) {
            memset(buffer, 0, sizeof(buffer));

            file.seekg(sizeof(buffer) * i);

            if (fileSize < BUFFER_SIZE) {
                file.read(buffer, fileSize);
                socket.async_send(boost::asio::buffer(buffer, fileSize), 0, [this](const boost::system::error_code& error, std::size_t bytes) {
                    _Config->write_handler_ptr(error, bytes); 
                });
            }
            else {
                file.read(buffer, sizeof(buffer));
                socket.async_send(boost::asio::buffer(buffer, BUFFER_SIZE), 0, [this](const boost::system::error_code& error, std::size_t bytes) {
                    _Config->write_handler_ptr(error, bytes); 
                });
            }
        }

        socket.async_send(boost::asio::buffer("end", 3), 0, [this](const boost::system::error_code& error, std::size_t bytes) {
            _Config->write_handler_ptr(error, bytes); 
        });

        file.close(); 
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, "(sendResponse) except: " + std::atoi(error.what()), 2);
        socket.async_send(boost::asio::buffer("end", 3), 0, [this](const boost::system::error_code& error, std::size_t bytes) {
            _Config->write_handler_ptr(error, bytes); 
        });
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, "(sendResponse) catch unknw error", 2);
        socket.async_send(boost::asio::buffer("end", 3), 0, [this](const boost::system::error_code& error, std::size_t bytes) {
            _Config->write_handler_ptr(error, bytes); 
        });
    }
    
    return 0;
}
