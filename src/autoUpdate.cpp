#include "../include/autoUpdate.h"
//#include "../include/sendResponse.h"
#include <fstream>
#include <mswsock.h>
#include <regex>

#pragma comment(lib, "mswsock.lib")

//SendResponse _SendResponse;
//AutoUpdate _AutoUpdate;
json m_latestBuildInfo;

AutoUpdate::AutoUpdate(){
    _Config = Config::get_instance();
    _Logger = Logger::get_instance();

    //_SendResponse = std::make_shared<SendResponse>();
}

//AutoUpdate::~AutoUpdate(){
//    delete _Config;
//    delete _Logger;
//
//    if (_SendResponse != nullptr) delete _SendResponse;
//}

std::size_t AutoUpdate::hash_file(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);

    if (!file) {
        return 0;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return std::hash<std::string>{}(content);
}

int AutoUpdate::parseFiles(std::filesystem::path directory, std::string parsingType){
    std::filesystem::path directoryWithLatestBuild = directory;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
        std::filesystem::path filePath = std::filesystem::relative(entry.path(), directory);

        if (!entry.is_regular_file()) {
            continue;
        }

        if (parsingType == "hash") {
            m_latestBuildInfo["fileHashes"][entry.path().lexically_normal().filename().string()] = hash_file(entry.path().string());
        }

        if (parsingType == "paths") {
            m_latestBuildInfo["filePaths"][entry.path().lexically_normal().filename().string()] = std::regex_replace(entry.path().string(), std::regex(directoryWithLatestBuild.string() + R"(\\)"), "");
        }
    }

    return 0;
}

int AutoUpdate::parseDirectories(std::filesystem::path directory) {
    std::filesystem::path directoryWithLatestBuild = directory;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
        if (entry.is_directory()) {
            //std::cout << entry.path().string() << std::endl;
            //filePath = std::regex_replace(filePath, std::regex(R"(\")"), "");
            m_latestBuildInfo["directoriesPaths"][entry.path().filename().string()] = std::regex_replace(entry.path().string(), std::regex(directoryWithLatestBuild.string() + R"(\\)"), "");
            //parseDirectories(entry);
        }
    }

    return 0;
}

int AutoUpdate::initializeLatestBuildInfo(){
    //Config _Config;

    //_Config.parseConfig();

    std::filesystem::path directory = _Config->lastClientBuildDir;

    //std::cout << directory << std::endl;

    if (!m_latestBuildInfo.contains("fileHashes")) {
        _Logger->addLog("WARN", "json block with fileHashes is empty", 2);
        parseFiles(directory, "hash");
    }

    if (!m_latestBuildInfo.contains("filePaths")) {
        _Logger->addLog("WARN", "json block with filePaths is empty", 2);
        parseFiles(directory, "paths");
    }

    //_SendResponse->sendJSON(m_responseJSON, "actualClientFilesHashes");

    Config::get_instance()->latestBuildInfo = m_latestBuildInfo;
    return 0;
}

int AutoUpdate::collectFilesInfo(){
    //Config _Config;
    //_Config.parseConfig();

    std::filesystem::path directoryName = _Config->lastClientBuildDir;

    if (!std::filesystem::is_directory(directoryName)) {
        _Logger->addLog("WARN", "problems with _Config.clientFilesPath", 2);
        return 1;
    }

    if (parseFiles(directoryName, "hash") == 0) {
        _Logger->addLog("INFO", "fileHashes successfully collected", 2);
    }

    if (parseFiles(directoryName, "paths") == 0) {
        _Logger->addLog("INFO", "filePaths successfully collected", 2);
    }

    if (parseDirectories(directoryName) == 0) {
        _Logger->addLog("INFO", "DirectoriesPaths successfully collected", 2);
    }

    //std::cout << std::endl << m_responseJSON.dump() << std::endl << std::endl;

    return 0;
}