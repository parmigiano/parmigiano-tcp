#ifndef AUTOUPDATE_H
#define AUTOUPDATE_H

#include <string>
#include <map>
#include <filesystem>
#include <WinSock2.h>
//#include <memory>

#include "../include/config.h"
#include "../include/logger.h"

//class SendResponse;

//class SendResponse

class AutoUpdate { 
private:
	Config* _Config;
	Logger* _Logger;

	std::size_t hash_file(const std::string& filePath);
	int parseFiles(std::filesystem::path directory, std::string parsingType, std::string mainFileNameServer, std::string mainFileNameClient);
	int parseDirectories(std::filesystem::path directory);
public:
	AutoUpdate();
	~AutoUpdate() = default;

	// int initializeLatestBuildInfo();
	int collectFilesInfoAndroid();
	int collectFilesInfoWin();
};

//extern AutoUpdate _AutoUpdate;

#endif 