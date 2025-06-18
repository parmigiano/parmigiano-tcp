#include "../include/requestProcessing.h"

#include "../json-develop/single_include/nlohmann/json.hpp"

#include "../include/sendResponse.h"
//#include "../include/autoUpdate.h"

//#include <iostream>

//RequestProcessing _requestProcessing;
//AutoUpdate _AutoUpdate;
//SendResponse _SendResponse;
//using json = nlohmann::json;

RequestProcessing::RequestProcessing(){
    _Logger = Logger::get_instance();

    //_AutoUpdate = std::make_shared<AutoUpdate>();
    _SendResponse = std::make_shared<SendResponse>();
}

//RequestProcessing::~RequestProcessing(){
//    delete _Logger;
//}

int RequestProcessing::requestDistribution(std::string requestStringJSON, SOCKET clientSocket){
    nlohmann::json j = nlohmann::json::parse(requestStringJSON);

    // std::cout << j.dump() << std::endl;

    if (!j.contains("requestInfo")) {
        _Logger->addLog("WARN", "request have not case: requestInfo", 2);
        return 1;
    }

    if (!j["requestInfo"].contains("requestType")) {
        _Logger->addLog("WARN", "case: requestInfo, have not key: requestType", 2);
        return 1;
    }

    if (j["requestInfo"]["requestType"] == "fileHashRequest") {
        _Logger->addLog("INFO", "Client: require actual files info", 2);
        //_AutoUpdate->initializeLatestBuildInfo();
        //_SendResponse->sendJSON(m_responseJSON, "actualClientFilesHashes");
        _SendResponse->sendJSON(Config::get_instance()->latestBuildInfo, "actualClientFilesHashes", clientSocket);
    }

    if (j["requestInfo"]["requestType"] == "downloadFile") {
        _Logger->addLog("INFO", "Client: require files", 2);

        for (auto& [fileName, filePath] : j["filesToDownload"].items()) {
            _SendResponse->sendFile(filePath, fileName, clientSocket);
        }
    }

    return 0;
}
