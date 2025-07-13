#include "../include/requestProcessing.h"

#include "../include/sendResponse.h"

RequestProcessing::RequestProcessing(){
    _Logger = Logger::get_instance();

    //_AutoUpdate = std::make_shared<AutoUpdate>();
    _SendResponse = std::make_shared<SendResponse>();
}

int RequestProcessing::requestDistribution(std::string requestStringJSON, SOCKET clientSocket){

    ClientRequestStruct::Request acceptedRequest;
    acceptedRequest.ParseFromString(requestStringJSON);

    if (acceptedRequest.requestinfo().type() == ClientRequestStruct::RequestInfo::fileHashRequest) {
        _Logger->addLog("INFO", "Client: require actual files info", 2);

        _SendResponse->setReponseType("filesHashes");

        _FileInfoFields::iterator it = Config::get_instance()->buildFilesInfo.begin();
        
        for (auto& [fileName, filePath, fileHash] : Config::get_instance()->buildFilesInfo) {
            _SendResponse->addFileInfo(fileHash, filePath, fileName);
        }

        for (auto& dirPath : Config::get_instance()->buildDirsInfo) {
            _SendResponse->addDirInfo(dirPath);
        }

        _SendResponse->sendResponse(clientSocket);
    }
    else if (acceptedRequest.requestinfo().type() == ClientRequestStruct::RequestInfo::downloadFile) {
        _Logger->addLog("INFO", "Client: require files", 2);

        for (int i = 0; i < acceptedRequest.filesinfo_size(); i++) {
            const ClientRequestStruct::FilesInfo& fileInfo = acceptedRequest.filesinfo(i);

            _SendResponse->sendFile(fileInfo.path(), fileInfo.name(), clientSocket);
        }

    }
    else {

    }

    return 0;
}
