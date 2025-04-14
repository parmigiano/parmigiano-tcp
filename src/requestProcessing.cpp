#include "../include/requestProcessing.h"
#include "../include/autoUpdate.h"
#include "../json-develop/single_include/nlohmann/json.hpp"

#include <iostream>

using json = nlohmann::json;

int requestProcessing::requestDistribution(std::string requestStringJSON){
    AutoUpdate _AutoUpdate;

	json j = json::parse(requestStringJSON);

    std::cout << j.dump() << std::endl;

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
        std::cout << j.dump() << std::endl;
    }

    return 0;
}
