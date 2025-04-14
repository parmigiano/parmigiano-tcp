#include "../include/serialization.h"
#include "../json-develop/single_include/nlohmann/json.hpp"

#include <iostream>

using json = nlohmann::json;

std::string Serialization::requestStructToJson(std::unordered_map<std::string, std::unordered_map<std::string, std::string>> requestData){

    if (requestData.empty()) {
        return "err";
    }

    json j;
    for (const auto& outerPair : requestData) {
        json innerJson;
        for (const auto& innerPair : outerPair.second) {
            innerJson[innerPair.first] = innerPair.second;
        }
        j[outerPair.first] = innerJson;
    }

	return j.dump(4);
}