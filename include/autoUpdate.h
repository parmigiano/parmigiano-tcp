#ifndef AUTOUPDATE_H
#define AUTOUPDATE_H

#include <string>
#include <filesystem>
#include <WinSock2.h>

#include "../json-develop/single_include/nlohmann/json.hpp"


namespace fs = std::filesystem;
using json = nlohmann::json;

class AutoUpdate {
private:
	std::size_t hash_file(const std::string& filePath);
	int parseFiles(fs::path directory, std::string parsingType);
	int parseDirectories(fs::path directory);
public:
	int sendRequiredFileHashes();
	int collectFilesInfo();
};

#endif 