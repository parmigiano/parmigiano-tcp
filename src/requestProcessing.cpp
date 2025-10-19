#include "../include/requestProcessing.h"

#include "../include/sendResponse.h"

#include <regex>

RequestProcessing::RequestProcessing(){
    _Logger = Logger::get_instance();
    _Config = Config::get_instance();
    
    _SendResponse = std::make_shared<SendResponse>();
}

int RequestProcessing::requestDistribution(std::string requestStr, boost::asio::ip::tcp::socket& socket){
    ClientRequestStruct::Request acceptedRequest;
    acceptedRequest.ParseFromString(requestStr);

    try {
        switch (acceptedRequest.requestinfo().type()) {
        case ClientRequestStruct::RequestInfo::fileHashRequest: // dispatches of file hashes
            _Logger->addServerLog(_Logger->info, "(requestProcessing) Client: require actual files info", 2);
            filesHashes(acceptedRequest, socket);
            break;

        case ClientRequestStruct::RequestInfo::downloadFile: // sending file
            _Logger->addServerLog(_Logger->info, "(requestProcessing) Client: require file", 2);
            file(acceptedRequest, socket);
            break;

        default:
            _Logger->addServerLog(_Logger->warn, "(requestProcessing) unknw request type", 2);
            break;
        }
    } 
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, "(requestProcessing) except: " + std::atoi(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, "(requestProcessing) catch unknw error", 2);
    }
    
    return 0;
}

void RequestProcessing::filesHashes(ClientRequestStruct::Request acceptedRequest, boost::asio::ip::tcp::socket& socket){
    _SendResponse->setReponseType("filesHashes");
    _SendResponse->addMainFileName(_Config->configurationVars["mainExeNameForClient"]);

    for (auto& [fileName, filePath, fileHash] : _Config->filesInfo) {
        _SendResponse->addFileInfo(fileHash, filePath, fileName);
    }

    for (auto& dirPath : _Config->buildDirsInfo) {
        _SendResponse->addDirInfo(dirPath);
    }

    _SendResponse->sendResponse(socket);
}

void RequestProcessing::file(ClientRequestStruct::Request acceptedRequest, boost::asio::ip::tcp::socket& socket){
    for (int i = 0; i < acceptedRequest.fileinfo_size(); i++) {
        const ClientRequestStruct::FileInfo& fileInfo = acceptedRequest.fileinfo(i);

        if (!strcmp(fileInfo.filename().c_str(), _Config->configurationVars["mainExeNameForClient"].c_str())) {
            _SendResponse->sendFile(std::regex_replace(fileInfo.filepath(), std::regex(fileInfo.filename()), _Config->configurationVars["mainExeNameForServer"]), socket);
        }
        else {
            _SendResponse->sendFile(fileInfo.filepath(), socket);
        }
    }
}
