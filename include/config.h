#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <mutex>
#include <boost/asio.hpp>
#include <functional>
#include <map>
#include <vector>

#include "../include/logger.h"

struct FilesInfoFields {
    std::string name;
    std::string path;
    std::string hash;
};

struct Config {
private:
    static Config* instance_ptr;
    static std::mutex mtx;

    Logger* _Logger;

    std::string configurationFileName = "Config.txt";

    void assigningValue(std::string key, std::string value);
    void varsFillingCheck();
    std::string checkAndRemoveBackspaces(std::string line);
public:
    Config();
    Config(const Config&) = delete;
    ~Config() = default;

    static Config* get_instance();
    
    // main config vars here
    std::map <std::string, std::string> configurationVars 
    {
        // with value "null" will be checking vars for exist in configuration file (for important vars)
        // with out "null" willnt check vars for exist (for not important vars)

        // Field for server setting
        {"serverPort", "null"},
        {"logDir", "null"},

        // Fields for update Windows systems
        {"buildDir", "null"},
        {"mainExeNameForClient", "null"},
        {"mainExeNameForServer", "null"}
    };

    // For send responses
    std::function<void(const boost::system::error_code& error, size_t bytes)> write_handler_ptr;

    // Data update info
    std::vector <FilesInfoFields> filesInfo;
    std::vector <std::string> buildDirsInfo;

    // List of most important files for update
    std::vector <std::string> impFilesNames = {
        "mainExeNameForClient"
    };

    void parseConfig();
};

#endif 