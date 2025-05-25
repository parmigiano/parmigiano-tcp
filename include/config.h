#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <iostream>
#include <map>

struct Config {
private:
    int isFile();
    void assigningValues(std::string key, std::string value);
public:
    short int serverPort = 0;
    std::string clientFilesPath = "";
    std::string logDir = "";
    int parseConfig();
};

extern Config _Config;

#endif 