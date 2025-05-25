#include "../include/autoUpdate.h"
#include "../include/config.h"
#include "../include/sendResponse.h"

#include <fstream>
#include <mswsock.h>
#include <regex>

#pragma comment(lib, "mswsock.lib")

json m_responseJSON;

std::size_t AutoUpdate::hash_file(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);

    if (!file) {
        return 0;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return std::hash<std::string>{}(content);
}

int AutoUpdate::parseFiles(fs::path directory, std::string parsingType){
    fs::path directoryWithLatestBuild = directory;

    /*for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        std::filesystem::path filePath = std::filesystem::relative(entry.path(), directory);
        if (entry.is_regular_file()) {
            if (parsingType == "hash")
                m_responseJSON["fileHashes"][entry.path().lexically_normal().filename().string()] = hash_file(entry.path().string());

            if (parsingType == "paths")
                m_responseJSON["filePaths"][entry.path().lexically_normal().filename().string()] = filePath.string();
        }
    }*/

    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        std::filesystem::path filePath = std::filesystem::relative(entry.path(), directory);

        if (entry.is_regular_file()) {
            if (parsingType == "hash")
                m_responseJSON["fileHashes"][entry.path().lexically_normal().filename().string()] = hash_file(entry.path().string());

            if (parsingType == "paths")
                m_responseJSON["filePaths"][entry.path().lexically_normal().filename().string()] = std::regex_replace(entry.path().string(), std::regex(directoryWithLatestBuild.string() + R"(\\)"), "");
        }
    }

    return 0;
}

int AutoUpdate::parseDirectories(fs::path directory) {
    fs::path directoryWithLatestBuild = directory;

    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_directory()) {
            //std::cout << entry.path().string() << std::endl;
            //filePath = std::regex_replace(filePath, std::regex(R"(\")"), "");
            m_responseJSON["directoriesPaths"][entry.path().filename().string()] = std::regex_replace(entry.path().string(), std::regex(directoryWithLatestBuild.string() + R"(\\)"), "");
            //parseDirectories(entry);
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
        parseFiles(directory, "hash");
    }

    if (!m_responseJSON.contains("filePaths")) {
        std::cout << "(!) json block with filePaths is empty" << std::endl;
        parseFiles(directory, "paths");
    }

    _SendResponse.sendJSON(m_responseJSON, "actualClientFilesHashes");
    return 0;
}

int AutoUpdate::collectFilesInfo(){
    Config _Config;
    _Config.parseConfig();

    fs::path directoryName = _Config.clientFilesPath;

    if (!fs::is_directory(directoryName)) {
        std::cout << "(!) problems with _Config.clientFilesPath" << std::endl;
        return 1;
    }

    if (parseFiles(directoryName, "hash") == 0) {
        std::cout << "(!) fileHashes successfully collected" << std::endl;
    }

    if (parseFiles(directoryName, "paths") == 0) {
        std::cout << "(!) filePaths successfully collected" << std::endl;
    }

    if (parseDirectories(directoryName) == 0) {
        std::cout << "(!) DirectoriesPaths successfully collected" << std::endl;
    }

    std::cout << std::endl << m_responseJSON.dump() << std::endl << std::endl;

    return 0;
}
