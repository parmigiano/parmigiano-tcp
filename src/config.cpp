#include "../include/config.h"
//#include "../include/logger.h"

#include <fstream>

Config* Config::instance_ptr = nullptr;
std::mutex Config::mtx;

Config* Config::get_instance() {
    std::lock_guard<std::mutex> lock(mtx);
    if (instance_ptr == nullptr) {
        instance_ptr = new Config();
    }

    return instance_ptr;
}

Config::~Config(){
    instance_ptr = nullptr;
}

int Config::isFile(){
    std::ofstream file;

    file.open("Config.txt", std::ios::app);

    /*if (!file.is_open()) {
        std::cerr << "Failed to open file Config.txt" << std::endl;
        file.close();
        return 1;
    }*/

    file.close();

    return 0;
}

void Config::assigningValues(std::string key, std::string value){
    if (key == "serverPort") {
        serverPort = std::stoi(value);
    }
    else if (key == "lastClientBuildDir") {
        lastClientBuildDir = value;
    } 
    else if (key == "logDir") {
        logDir = value;
    }
}

int Config::parseConfig(){
    isFile();

    std::string line;
    
    std::ifstream file("Config.txt");
    if (!file.is_open()) {
        //_Logger.addLog("WARN", "Failed to open file Config.txt for reading", 2);
        file.close();
        return 1;
    }

    if (file.peek() == EOF) {
        //_Logger.addLog("WARN", "Config file is empty!", 2);
        return 1;
    }

    while (std::getline(file, line)) {
        std::string key = line.substr(0, line.find("=")), value = line.substr(line.find("=") + 1, line.back());

        if (value.empty()) {
            //_Logger.addLog("WARN", "key: " + key + " must include value!", 2);
            return 1;
        }

        assigningValues(key, value);
    }

    file.close();

	return 0;
}
