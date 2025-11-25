#pragma once

//#include "logger.h"

#include <string>
#include <mutex>
#include <boost/asio.hpp>
#include <functional>
#include <map>
#include <vector>
#include <fstream>

struct Config {
public:
    enum fileTypes {
        config,
        env
    };
private:
    //Logger* _Logger;

    static Config* instance_ptr_;
    static std::mutex mtx_;

    // std::vector<std::string> env_parse_names_ = { "DB_USER", "DB_PASSWORD", "DB_URL" };
    std::map<std::string, std::string> temp_vars_develop_;
    std::map<std::string, std::string> temp_vars_production_;

    std::string configuration_file_name_ = "Config.txt";
    const std::string MODULE_NAME_ = "(Config)";
    bool is_init_ = false;
public:
    Config();
    Config(const Config&) = delete;
    ~Config() = default;

    static Config* get_instance();
    
    // main config vars here
    std::map<std::string, std::string> configuration_vars_ {
        // with value "null" will be checking vars for exist in configuration file (for important vars)
        // with out "null" willnt check vars for exist (for not important vars)

        // Field for server setting
        // Config.txt
        {"mode", "null"}, // develop or production
        {"server_port", "null"},
        {"log_dir", "null"}, 
        {"log_server_subdir", "null"},
        {"log_session_subdir", "null"},
        {"session_live_time", "null"}, // in minutes
        

        // .env
        {"DB_USER", "null"},
        {"DB_PASSWORD", "null"},
        {"DB_ADDRESS", "null"},
        {"DB_PORT", "null"},
        {"DB_NAME", "null"}
    };

    // For send responses
    std::function<void(const boost::system::error_code& error, size_t bytes)> write_handler_ptr;

    std::string toLower(std::string str);
    std::ifstream openFile(std::string filename);
    std::string getPathByType(fileTypes);
    void parseFrom(fileTypes);
    void mergeVars();
    void varsFillingCheck();
    void initialize();
    bool isInitialize();
};