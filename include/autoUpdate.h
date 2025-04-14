#ifndef AUTOUPDATE_H
#define AUTOUPDATE_H

#include <string>
#include <filesystem>

#include "../json-develop/single_include/nlohmann/json.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

class AutoUpdate {
private:
	std::size_t hash_file(const std::string& filePath);
	int parseDirectory(fs::path directory, std::string parsingType);
public:
	int sendRequiredFileHashes();
	int collectFilesInfo();
};

#endif 