#include "../include/requestProcessing.h"
#include "../include/sendResponse.h"
#include "../include/autoUpdate.h"
#include "../json-develop/single_include/nlohmann/json.hpp"

#include <iostream>

using json = nlohmann::json;

int requestProcessing::requestDistribution(std::string requestStringJSON){
    AutoUpdate _AutoUpdate;
    SendResponse _SendResponse;


	json j = json::parse(requestStringJSON);

    // std::cout << j.dump() << std::endl;

    if (!j.contains("requestInfo")) {
        std::cout << "(!) request have not case: requestInfo" << std::endl;
        return 1;
    }

    if (!j["requestInfo"].contains("requestType")) {
        std::cout << "(!) case: requestInfo, have not key: requestType" << std::endl;
        return 1;
    }

    if (j["requestInfo"]["requestType"] == "fileHashRequest") {
        _AutoUpdate.sendRequiredFileHashes();
    }

    if (j["requestInfo"]["requestType"] == "downloadFile") {
        json fileInfo;

        //fileInfo["responseInfo"]["sendindStatus"] = "start";
        //_SendResponse.sendJSON(fileInfo, "downloadFile");
        //fileInfo.clear();

        for (auto& [fileName, filePath] : j["filesToDownload"].items()) {

            //fileInfo["responseInfo"]["sendindStatus"] = "processing";

            //fileInfo["fileInfo"]["filePath"] = filePath;
            //fileInfo["fileInfo"]["fileName"] = fileName;

            //_SendResponse.sendJSON(fileInfo, "downloadFile");
            _SendResponse.sendFile(filePath, fileName);
            fileInfo.clear();
            //Sleep(10);
        }

        /*fileInfo["responseInfo"]["sendindStatus"] = "end";
        _SendResponse.sendJSON(fileInfo, "downloadFile");*/

        //fileInfo.clear();

    }

    return 0;
}
