#ifndef SENDRESPONSE_H
#define SENDRESPONSE_H

#include "../json-develop/single_include/nlohmann/json.hpp"

#include <string>
#include <iostream>
#include <map>

using json = nlohmann::json;

struct SendResponse {
private:
public:
	int sendJSON(json j, std::string responseType);
	int sendFile(std::string filePath, std::string fileName);
};

#endif 