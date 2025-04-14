#include "../include/config.h"

#include <fstream>

int Config::isFile(){
    std::ofstream file;

    file.open("Config.txt", std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open file Config.txt" << std::endl;
        file.close();
        return 1;
    }

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
}

int Config::parseConfig(){
    isFile();

    std::string line;
    
    std::ifstream file("Config.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open file Config.txt for reading" << std::endl;
        file.close();
        return 1;
    }

    while (std::getline(file, line)) {
        std::string key = line.substr(0, line.find("=")), value = line.substr(line.find("=") + 1, line.back());

        if (value.empty()) {
            std::cerr << "(!) ERROR! key: \"" << key << "\" must include value!" << std::endl;
            continue;
        }

        assigningValues(key, value);
    }

    file.close();

	return 0;
}
