#include "../include/autoUpdate.h"

#include <fstream>
#include <mswsock.h>
#include <regex>
#include <vector>

#pragma comment(lib, "mswsock.lib")

std::vector <std::string> filesHashesVec;
std::vector <std::string> filesPathsVec;
std::vector <std::string> filesNamesVec; // change to multidimensional vector
// std::vector <std::vector<std::string>> filesInfoVec; { {fileHash, filePath, fileName}, ... }

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

int AutoUpdate::parseFiles(std::filesystem::path directory, std::string parsingType, std::string mainFileNameServer, std::string mainFileNameClient){
    std::filesystem::path directoryWithLatestBuild = directory;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
        std::filesystem::path filePath = std::filesystem::relative(entry.path(), directory);

        if (!entry.is_regular_file()) {
            continue;
        }

        if (parsingType == "hash") {
            filesHashesVec.push_back(std::to_string(hash_file(entry.path().string())));
            continue;
        }

        if (parsingType == "path" && strcmp(entry.path().lexically_normal().filename().string().c_str(), mainFileNameServer.c_str())) {
            filesPathsVec.push_back(std::regex_replace(entry.path().string(), std::regex(directoryWithLatestBuild.string() + R"(\\)"), ""));
        }
        else {
            filePath = filePath.parent_path().string() + mainFileNameClient.c_str(); // problem with    path + "/" + path

            filesPathsVec.push_back(std::regex_replace(filePath.string(), std::regex(directoryWithLatestBuild.string() + R"(\\)"), ""));
        }

        auto it = std::find(filesNamesVec.begin(), filesNamesVec.end(), filePath.filename().string());

        if (it == filesNamesVec.end()) {
            filesNamesVec.push_back(filePath.filename().string());
        }
    }

    return 0;
}

int AutoUpdate::parseDirectories(std::filesystem::path directory) {
    std::filesystem::path directoryWithLatestBuild = directory;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
        if (entry.is_directory()) {
            dirsPathsVec.push_back(std::regex_replace(entry.path().string(), std::regex(directory.string() + R"(\\)"), ""));
        }
    }

    return 0;
}

int AutoUpdate::collectFilesInfoAndroid(){
    return 0;
}

int AutoUpdate::collectFilesInfoWin(){

    if (!std::filesystem::is_directory(_Config->buildDirWin)) {
        std::filesystem::create_directory(_Config->buildDirWin);
        _Logger->addLog("INFO", "(autoUpdate) created dir for latest client build. Move files to inside dir", 2);
        return 1;
    }

    if (parseFiles(_Config->buildDirWin, "hash", _Config->mainExeNameForServerWin, _Config->mainExeNameForClientWin) == 0) {
        _Logger->addLog("INFO", "(autoUpdate) fileHashes successfully collected", 2);
    }

    if (parseFiles(_Config->buildDirWin, "path", _Config->mainExeNameForServerWin, _Config->mainExeNameForClientWin) == 0) {
        _Logger->addLog("INFO", "(autoUpdate) filePaths successfully collected", 2);
    }

    if (parseDirectories(_Config->buildDirWin) == 0) {
        _Logger->addLog("INFO", "(autoUpdate) DirectoriesPaths successfully collected", 2);
    }

    _Logger->addLog("INFO", "(autoUpdate) name\t\tpath\t\thash", 2);

    for (int i = 0; i < filesNamesVec.size(); i++) {
        _Logger->addLog("INFO", "(autoUpdate) " + filesNamesVec[i] + "\t" + filesPathsVec[i] + "\t" + filesHashesVec[i], 2);
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