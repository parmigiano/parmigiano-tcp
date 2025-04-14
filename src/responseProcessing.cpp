#include "../include/responseProcessing.h"
#include "../include/autoUpdate.h"
#include "../json-develop/single_include/nlohmann/json.hpp"

#include <iostream>

using json = nlohmann::json;

int ResponseProcessing::responseDistribution(std::string responseStringJSON){

    json j = json::parse(responseStringJSON);

    if (!j.contains("requestInfo")) {
        std::cout << "(!) request have not case: requestInfo";
        return 1;
    }

    if (!j["requestInfo"].contains("requestType")) {
        std::cout << "(!) case: requestInfo, have not key: requestType";
        return 1;
    }

    /*if (j["requestInfo"]["requestType"] == "actualClientFilesHashes") {
        AutoUpdate _AutoUpdate;
        _AutoUpdate.comparisonHash(responseStringJSON);
    }*/

	return 0;
}
