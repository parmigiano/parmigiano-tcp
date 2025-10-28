#include "../include/autoUpdate.h"

#include <fstream>
#include <mswsock.h>
#include <regex>
#include <vector>
#include <string>

AutoUpdate::AutoUpdate(){
    _Config = Config::get_instance();
    _Logger = Logger::get_instance();
}

std::size_t AutoUpdate::hash_file(const std::string& filePath){
    std::ifstream file(filePath, std::ios::binary);

    if (!file) {
        return 0;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return std::hash<std::string>{}(content);
}

int AutoUpdate::parseFiles(std::filesystem::path directory, std::string mainFileNameServer, std::string mainFileNameClient){
    std::filesystem::path directoryWithLatestBuild = directory;

    try {
        std::string name, path, hash;

        for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
            std::filesystem::path filePath = std::filesystem::relative(entry.path(), directory);

            if (!entry.is_regular_file()) {
                continue;
            }

            hash = std::to_string(hash_file(entry.path().string()));
            path = std::regex_replace(entry.path().string(), std::regex(directoryWithLatestBuild.string() + R"(\\)"), "");

            if (strcmp(entry.path().lexically_normal().filename().string().c_str(), mainFileNameServer.c_str())) {
                path = std::regex_replace(entry.path().string(), std::regex(directoryWithLatestBuild.string() + R"(\\)"), "");
            }
            else {
                filePath = filePath.parent_path().string() + mainFileNameClient.c_str(); // problem with    path + "/" + path

                path = std::regex_replace(filePath.string(), std::regex(directoryWithLatestBuild.string() + R"(\\)"), "");
            }

            name = filePath.filename().string();

            _Config->filesInfo.push_back( { name, path, hash } );
        }

        return 0;
    }
    catch (std::filesystem::filesystem_error error) {
        _Logger->addServerLog(_Logger->info, "(autoUpdate) parse files ends with error: " + std::atoi(error.what()), 2);
        return 1;
    }
    catch (...) {
        return 1;
    }

}

int AutoUpdate::parseDirectories(std::filesystem::path directory){
    std::filesystem::path directoryWithLatestBuild = directory;

    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
            if (entry.is_directory()) {
                _Config->buildDirsInfo.push_back(std::regex_replace(entry.path().string(), std::regex(directory.string() + R"(\\)"), ""));
            }
        }
    } 
    catch (std::filesystem::filesystem_error error) {
        _Logger->addServerLog(_Logger->info, "(autoUpdate) parse directories ends with error: " + std::atoi(error.what()), 2);
        return 1;
    } 
    catch (...) {
        return 1;
    }

    return 0;
}

int AutoUpdate::checkImpFiles(){
    for (auto& name : _Config->impFilesNames) {
        auto it = std::find_if(_Config->filesInfo.begin(), _Config->filesInfo.end(), [&] (FilesInfoFields& filesInfo) {
            return _Config->configurationVars[name] == filesInfo.name;
        });

        if (it == _Config->filesInfo.end()) return 1;
    }

    return 0;
}

void AutoUpdate::displayCollectedFilesInfo() {
    _Logger->addServerLog(_Logger->info, "(autoUpdate)", 2);
    _Logger->addServerLog(_Logger->info, "(autoUpdate) name\t\tpath\t\thash", 2);

    for (auto& file : _Config->filesInfo) {
        _Logger->addServerLog(_Logger->info, "(autoUpdate) " + file.name + "\t" + file.path + "\t" + file.hash, 2);
    }
}

void AutoUpdate::collectFilesInfo(){
    if (!std::filesystem::is_directory(_Config->configurationVars["buildDir"])) {
        std::filesystem::create_directory(_Config->configurationVars["buildDir"]);
        _Logger->addServerLog(_Logger->info, "(autoUpdate) created dir for latest client build. Move files to inside dir", 2);
        exit(1);
    }

    if (parseFiles(_Config->configurationVars["buildDir"], _Config->configurationVars["mainExeNameForServer"], _Config->configurationVars["mainExeNameForClient"]) == 0) {
        _Logger->addServerLog(_Logger->info, "(autoUpdate) files info successfully collected", 2);
    }

    if (parseDirectories(_Config->configurationVars["buildDir"]) == 0) {
        _Logger->addServerLog(_Logger->info, "(autoUpdate) Directories paths successfully collected", 2);
    }

    displayCollectedFilesInfo();

    if (checkImpFiles() == 1) {
        _Logger->addServerLog(_Logger->error, "(autoUpdate) build havent important client file. Like: .exe, configs and more", 2);
        exit(1);
    }
}