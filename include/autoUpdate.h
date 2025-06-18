#ifndef AUTOUPDATE_H
#define AUTOUPDATE_H

#include <string>
#include <filesystem>
#include <WinSock2.h>
//#include <memory>

#include "../json-develop/single_include/nlohmann/json.hpp"
#include "../include/config.h"
#include "../include/logger.h"

//class SendResponse;

//class SendResponse;

using json = nlohmann::json;

class AutoUpdate { 
private:
	Config* _Config;
	Logger* _Logger;

	//SendResponse* _SendResponse = nullptr;
	//std::shared_ptr<SendResponse> _SendResponse;

	std::size_t hash_file(const std::string& filePath);
	int parseFiles(std::filesystem::path directory, std::string parsingType);
	int parseDirectories(std::filesystem::path directory);
public:
	AutoUpdate();
	~AutoUpdate() = default;

	int initializeLatestBuildInfo();
	int collectFilesInfo();
};

//extern AutoUpdate _AutoUpdate;

#endif 