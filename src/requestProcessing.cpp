#include "../include/requestProcessing.h"

#include "../include/sendResponse.h"

#include <regex>

RequestProcessing::RequestProcessing(){
    _Logger = Logger::get_instance();
    _Config = Config::get_instance();

    //_AutoUpdate = std::make_shared<AutoUpdate>();
    _SendResponse = std::make_shared<SendResponse>();
}

int RequestProcessing::requestDistribution(std::string requestStringJSON, SOCKET clientSocket){

    ClientRequestStruct::Request acceptedRequest;
    acceptedRequest.ParseFromString(requestStringJSON);

    if (acceptedRequest.requestinfo().type() == ClientRequestStruct::RequestInfo::fileHashRequest) { // form of answer of file hashes
        _Logger->addLog("INFO", "Client: require actual files info", 2);

        _SendResponse->setReponseType("filesHashes");
        _SendResponse->addMainFileName(_Config->mainExeNameForClientWin);

        _FileInfoFields::iterator it = _Config->buildFilesInfo.begin();
        
        for (auto& [fileName, filePath, fileHash] : _Config->buildFilesInfo) {
            _SendResponse->addFileInfo(fileHash, filePath, fileName);
        }

        for (auto& dirPath : _Config->buildDirsInfo) {
            _SendResponse->addDirInfo(dirPath);
        }

        _SendResponse->sendResponse(clientSocket);
    }
    else if (acceptedRequest.requestinfo().type() == ClientRequestStruct::RequestInfo::downloadFile) { // form of sending file
        _Logger->addLog("INFO", "Client: require files", 2);

        for (int i = 0; i < acceptedRequest.fileinfo_size(); i++) {
            const ClientRequestStruct::FileInfo& fileInfo = acceptedRequest.fileinfo(i);

            if (!strcmp(fileInfo.filename().c_str(), _Config->mainExeNameForClientWin.c_str())) {
                _SendResponse->sendFile(std::regex_replace(fileInfo.filepath(), std::regex(fileInfo.filename()), _Config->mainExeNameForServerWin), fileInfo.filename(), clientSocket);
            }
            else {
                _SendResponse->sendFile(fileInfo.filepath(), fileInfo.filename(), clientSocket);
            }
        }

    }
    else {

    }

    return 0;
}
