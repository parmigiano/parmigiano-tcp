#include "../include/sendResponse.h"

#include <regex>
#include <fstream>
#include <filesystem>

SendResponse::SendResponse(){
    _Logger = Logger::get_instance();
    _Config = Config::get_instance();
    
    _Response = std::make_shared<ResponseStruct::Response>();
}

void SendResponse::setReponseType(std::string responseType){
    if (responseType == "filesHashes") {
        _Response->mutable_responseinfo()->set_type(ResponseStruct::ResponseInfo::filesHashes);
    }
    else if (responseType == "downloadFile") {
        _Response->mutable_responseinfo()->set_type(ResponseStruct::ResponseInfo::downloadFile);
    }
    else {
        _Logger->addServerLog(_Logger->warn, "(sendResponse) unknown response type", 2);
    }
}

void SendResponse::addMainFileName(std::string fileName){
    _Response->mutable_nameofmainexefile()->set_filename(fileName);
}

void SendResponse::addFileInfo(std::string hash, std::string path, std::string name){
    auto* fileInfo = _Response->add_fileinfo();

    fileInfo->set_filehash(hash);
    fileInfo->set_filepath(path);
    fileInfo->set_filename(name);
}

void SendResponse::addDirInfo(std::string dirPath){
    auto* dirInfo = _Response->add_dirinfo();

    dirInfo->set_dirpath(dirPath);
}

void SendResponse::clearResponse(){
    _Response->Clear();
}

bool SendResponse::checkFileAvaibility(std::string filePath){
    for (auto& fileInfo : _Config->filesInfo) {
        if (fileInfo.path == filePath) return true;
    }

    return false;
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

    socket.async_send(boost::asio::buffer(data.c_str(), (int)data.size()), 0, [this](const boost::system::error_code& error, std::size_t bytes) {_Config->write_handler_ptr(error, bytes); });

    clearResponse();

	return 0;
}

int SendResponse::sendFile(std::string filePath, boost::asio::ip::tcp::socket& socket){
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    size_t chunk_count = 0;
    std::streampos currently_ptr_position;

    /*if (!checkFileAvaibility(filePath)) {
        _Logger->addServerLog(_Logger->warn, "(sendResponse) Client wanna get file outside allowed", 2);
        socket.async_send(boost::asio::buffer("end", 3), 0, [this](const boost::system::error_code& error, std::size_t bytes) {_Config->write_handler_ptr(error, bytes); });

        return 1;
    }*/

    filePath = _Config->configurationVars["buildDir"] + "/" + filePath;

    std::ifstream file(filePath, std::ios::in | std::ios::binary); // change to: open files in advance and parse them for possible errors

    if (!file) { // move it to a separate function
        _Logger->addServerLog(_Logger->warn, "(sendResponse) Failed to open file: " + filePath, 2);
        socket.async_send(boost::asio::buffer("end", 3), 0, [this](const boost::system::error_code& error, std::size_t bytes) {_Config->write_handler_ptr(error, bytes); });
        return 1;
    }

    unsigned int file_size = std::filesystem::file_size(filePath);

    if (file_size == 0) { // move it to a separate function and add to list of empty files
        _Logger->addServerLog(_Logger->warn, "(sendResponse) empty file " + filePath, 2);
        socket.async_send(boost::asio::buffer("end", 3), 0, [this](const boost::system::error_code& error, std::size_t bytes) {_Config->write_handler_ptr(error, bytes); });
        return 0;
    }

    while (true) {
        memset(buffer, 0, sizeof(buffer));

        file.seekg(sizeof(buffer) * chunk_count);

        if (file_size < BUFFER_SIZE) {
            file.read(buffer, file_size);
            socket.async_send(boost::asio::buffer(buffer, file_size), 0, [this](const boost::system::error_code& error, std::size_t bytes) {_Config->write_handler_ptr(error, bytes); });
        }
        else {
            file.read(buffer, sizeof(buffer));
            socket.async_send(boost::asio::buffer(buffer, BUFFER_SIZE), 0, [this](const boost::system::error_code& error, std::size_t bytes) {_Config->write_handler_ptr(error, bytes); });
        }

        currently_ptr_position = file.tellg();
        
        chunk_count++;

        file.seekg(0, std::ios::end);

        if (currently_ptr_position == file.tellg()) break;
    }

    socket.async_send(boost::asio::buffer("end", 3), 0, [this](const boost::system::error_code& error, std::size_t bytes) {_Config->write_handler_ptr(error, bytes); });

    file.close(); // change to: open files in advance and parse them for possible errors
    
    return 0;
}
