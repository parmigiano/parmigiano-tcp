#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <iostream>
#include <map>
#include <mutex>

#include "../json-develop/single_include/nlohmann/json.hpp"

struct Config {
private:
    static Config* instance_ptr;
    static std::mutex mtx;

    Config(){}

    int isFile();
    void assigningValues(std::string key, std::string svalue);
public:
    Config(const Config&) = delete;
    ~Config();

    static Config* get_instance();

    short int serverPort = 0;
    std::string lastClientBuildDir = "";
    std::string logDir = "";
    nlohmann::json latestBuildInfo;
    int parseConfig();
};

#endif 