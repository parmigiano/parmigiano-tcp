#ifndef AUTOUPDATE_H
#define AUTOUPDATE_H

#include <string>
#include <map>
#include <filesystem>
#include <WinSock2.h>
//#include <memory>

#include "../include/config.h"
#include "../include/logger.h"

struct fileInfoStruct {
	std::string name;
	std::string path;
	std::string hash;
};

//class SendResponse;
class AutoUpdate { 
private:
	Config* _Config;
	Logger* _Logger;

	std::size_t hash_file(const std::string& filePath);
	int parseFiles(std::filesystem::path directory, std::string mainFileNameServer, std::string mainFileNameClient);
	int parseDirectories(std::filesystem::path directory);
	int checkImpFiles();
	void displayCollectedFilesInfo();
public:
	AutoUpdate();
	~AutoUpdate() = default;

	void collectFilesInfo();
};

//extern AutoUpdate _AutoUpdate;

#endif 