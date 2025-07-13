#include "../include/autoUpdate.h"
//#include "../include/sendResponse.h"
#include <fstream>
#include <mswsock.h>
#include <regex>
#include <vector>

#pragma comment(lib, "mswsock.lib")

//SendResponse _SendResponse;
//AutoUpdate _AutoUpdate;
//json m_latestBuildInfo;

std::vector <std::string> filesHashesVec;
std::vector <std::string> filesPathsVec;
std::vector <std::string> filesNamesVec;
std::vector <std::string> dirsPathsVec;

AutoUpdate::AutoUpdate(){
    _Config = Config::get_instance();
    _Logger = Logger::get_instance();
}

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
            //m_latestBuildInfo["fileHashes"][entry.path().lexically_normal().filename().string()] = hash_file(entry.path().string());
            filesHashesVec.push_back(std::to_string(hash_file(entry.path().string())));
        }

        if (parsingType == "paths") {
            //m_latestBuildInfo["filePaths"][entry.path().lexically_normal().filename().string()] = std::regex_replace(entry.path().string(), std::regex(directoryWithLatestBuild.string() + R"(\\)"), "");
            filesPathsVec.push_back(std::regex_replace(entry.path().string(), std::regex(directoryWithLatestBuild.string() + R"(\\)"), ""));
        }

        auto it = std::find(filesNamesVec.begin(), filesNamesVec.end(), entry.path().lexically_normal().filename().string());

        if (it == filesNamesVec.end()) {
            filesNamesVec.push_back(entry.path().lexically_normal().filename().string());
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
            //m_latestBuildInfo["directoriesPaths"][entry.path().filename().string()] = std::regex_replace(entry.path().string(), std::regex(directory.string() + R"(\\)"), "");
            //parseDirectories(entry);
            dirsPathsVec.push_back(std::regex_replace(entry.path().string(), std::regex(directory.string() + R"(\\)"), ""));
        }
    }

    return 0;
}

//int AutoUpdate::initializeLatestBuildInfo(){
//    //Config _Config;
//        //_Config.parseConfig();
//
//    //filesHashesMap->find("fd");
//
//    std::filesystem::path directory = _Config->lastClientBuildDir;
//
//    if (!m_latestBuildInfo.contains("fileHashes")) {
//        _Logger->addLog("WARN", "json block with fileHashes is empty", 2);
//        parseFiles(directory, "hash");
//    }
//
//    if (!m_latestBuildInfo.contains("filePaths")) {
//        _Logger->addLog("WARN", "json block with filePaths is empty", 2);
//        parseFiles(directory, "paths");
//    }
//
//    std::string str = std::to_string(filesHashesVec.size()) + " " + std::to_string(filesPathsVec.size()) + " " + std::to_string(filesNamesVec.size()) + " ";
//
//    _Logger->addLog("INFO", str, 2);
//
//
//    //_SendResponse->sendJSON(m_responseJSON, "actualClientFilesHashes");
//
//    // Config::get_instance()->latestBuildInfo = m_latestBuildInfo;
//    return 0;
//}

int AutoUpdate::collectFilesInfo(){
    //Config _Config;
    //_Config.parseConfig();

    std::filesystem::path directoryName = _Config->buildDir;

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

    for (int i = 0; i < filesNamesVec.size(); i++) {
        Config::get_instance()->buildFilesInfo.push_back({ filesNamesVec[i], filesPathsVec[i], filesHashesVec[i] });
    }

    for (int i = 0; i < dirsPathsVec.size(); i++) {
        Config::get_instance()->buildDirsInfo.push_back(dirsPathsVec[i]);
    }

    filesHashesVec.clear();
    filesPathsVec.clear();
    filesNamesVec.clear();
    dirsPathsVec.clear();
 
    return 0;
}