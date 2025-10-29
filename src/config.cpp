#include "../include/config.h"

Config* Config::instance_ptr = nullptr;
std::mutex Config::mtx;

Config::Config() {
    _Logger = Logger::get_instance();
}

Config* Config::get_instance() {
    std::lock_guard<std::mutex> lock(mtx);
    if (instance_ptr == nullptr) {
        instance_ptr = new Config();
    }

    return instance_ptr;
}

void Config::assigningValue(std::string key, std::string value) {
    configurationVars[key] = value;
}

void Config::varsFillingCheck(){
    for (auto& [key, value] : configurationVars) {
        if (value == "null") {
            throw std::runtime_error("Important Key: \"" + key + "\" isnt added in config. Please solve this problem");
        }
    }
}

std::string Config::checkAndRemoveBackspaces(std::string line){
    std::string returnLine = "";

    if (line.find(" ") == std::string::npos) return line;

    for (char ch : line) if (ch != ' ') returnLine += ch;

    return returnLine;
}

void Config::parseConfig(){
    std::string line;
    
    try {
        std::ifstream file(configurationFileName);

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file Config.txt for reading");
            file.close();
        }

        if (file.peek() == EOF) {
            throw std::runtime_error("Config file is empty!");
        }

        while (std::getline(file, line)) {
            if (line.empty() || (line = checkAndRemoveBackspaces(line)) == "") { // empty line
                continue;
            }

            std::string key = line.substr(0, line.find("=")), value = line.substr(line.find("=") + 1, line.back());

            if (configurationVars.find(key) == configurationVars.end()) { // config not include this key. Marked as unknw key and throw error
                throw std::runtime_error("Unknw key: \"" + key + "\"");
            }

            if (line.find("=") == std::string::npos || value.empty()) { // key in config isnt init 
                throw std::runtime_error("Key: \"" + key + "\" isnt initialize. Please enter value like: \"" + key + "=value\"");
            }

            assigningValue(key, value);
        }

        varsFillingCheck();
        file.close();

    } catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->error, "(config) " + std::string(error.what()), 2);
        exit(1);
    } catch (...) {
        _Logger->addServerLog(_Logger->error, "(config) catch unknw error", 2);
        exit(1);
    }
}
