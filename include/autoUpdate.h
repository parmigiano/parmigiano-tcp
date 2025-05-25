#ifndef AUTOUPDATE_H
#define AUTOUPDATE_H

#include "../json-develop/single_include/nlohmann/json.hpp"

#include <string>
//#include <filesystem> 
#include <map>
#include <filesystem>

//namespace fs = std::filesystem;

using json = nlohmann::json;

class AutoUpdate {
private:
	std::string getHashLatestBuildFromServer();
	json getHashFromCurrentClientFiles();

	std::size_t hash_file(const std::string& filePath);

	int downloadFiles();
	int processAndSaveFile(int fileCreateFlag, std::string filePath, std::string fileName);
	int directoriesExist(json actualHashes);
	int comparisonHash(json clientHashes, json actualHashes);

public:
	int checkUpdate();
	int downloadUpdate();
};

#endif 
