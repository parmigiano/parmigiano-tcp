#ifndef AUTOUPDATE_H
#define AUTOUPDATE_H

#include "../json-develop/single_include/nlohmann/json.hpp"

#include <string>
//#include <filesystem> 
#include <map>

//namespace fs = std::filesystem;

using json = nlohmann::json;

class AutoUpdate {
private:
	std::string getHashLatestBuildFromServer();
	json getHashFromCurrentClientFiles();

	std::size_t hash_file(const std::string& filePath);

	int downloadFiles();
	int recordingFile(std::string acceptedBytes);

public:
	int comparisonHash(json clientHashes, json actualHashes);
	int checkUpdate();
	int downloadUpdate();
};

#endif 
