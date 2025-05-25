#include "../include/config.h"
#include "../include/logger.h"

#include <fstream>

Config _Config;

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
    else if (key == "clientFilesPath") {
        clientFilesPath = value;
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
        _Logger.addLog("PROBLEM", "Failed to open file Config.txt for reading", 1);
        file.close();
        return 1;
    }

    if (file.peek() == EOF) {
        std::cout << "File is empty." << std::endl;
    }

    while (std::getline(file, line)) {
        std::string key = line.substr(0, line.find("=")), value = line.substr(line.find("=") + 1, line.back());

        if (value.empty()) {
            _Logger.addLog("PROBLEM", "key: \"" + key + "\" must include value!", 1);
            continue;
        }

        assigningValues(key, value);
    }

    file.close();

	return 0;
}
