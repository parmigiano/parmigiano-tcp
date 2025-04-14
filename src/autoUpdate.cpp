#include "../include/autoUpdate.h"
#include "../include/config.h"
#include "../include/sendResponse.h"

#include <fstream>

json m_responseJSON;

std::size_t AutoUpdate::hash_file(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) return 0;

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return std::hash<std::string>{}(content);
}

int AutoUpdate::parseDirectory(fs::path directory, std::string parsingType){

    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            if(parsingType == "hash")
                m_responseJSON["fileHashes"][entry.path().filename().string()] = hash_file(entry.path().string());

            if (parsingType == "paths")
                m_responseJSON["filePaths"][entry.path().filename().string()] = entry.path().string();
        }
        else if (entry.is_directory()) {
            parseDirectory(entry, parsingType);
        }
    }

    return 0;
}

int AutoUpdate::sendRequiredFileHashes(){
    SendResponse _SendResponse;
    Config _Config;

    _Config.parseConfig();

    fs::path directory = _Config.clientFilesPath;

    if (!m_responseJSON.contains("fileHashes")) {
        std::cout << "(!) json block with fileHashes is empty" << std::endl;
        parseDirectory(directory, "hash");
    }

    if (!m_responseJSON.contains("filePaths")) {
        std::cout << "(!) json block with filePaths is empty" << std::endl;
        parseDirectory(directory, "paths");
    }

    _SendResponse.sendJSON(m_responseJSON);
    return 0;
}

int AutoUpdate::collectFilesInfo(){
    Config _Config;
    _Config.parseConfig();

    fs::path directory = _Config.clientFilesPath;

    if (!fs::is_directory(directory)) {
        std::cout << "(!) problems with _Config.clientFilesPath" << std::endl;
        return 1;
    }

    if (parseDirectory(directory, "hash") == 0) {
        std::cout << "(!) fileHashes successfully collected" << std::endl;
    }

    if (parseDirectory(directory, "paths") == 0) {
        std::cout << "(!) filePaths successfully collected" << std::endl;
    }

    return 0;
}
