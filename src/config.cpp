#include "config.h"

//#include <iostream> /////////////////////////////////

Config* Config::instance_ptr_ = nullptr;
std::mutex Config::mtx_;

Config::Config() {
    //_Logger = Logger::get_instance();

    //if (!_Logger->isInitalized()) _Logger->initialize();
}

Config* Config::get_instance() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (instance_ptr_ == nullptr) {
        instance_ptr_ = new Config();
    }

    return instance_ptr_;
}

std::string Config::toLower(std::string str) {
    std::string out;

    for (char& ch : str) out += std::tolower(ch);
    
    return out;
}

std::ifstream Config::openFile(std::string filename) {
    try {
        std::ifstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file Config.txt for reading");
            file.close();
        }

        if (file.peek() == EOF) {
            throw std::runtime_error("Config file is empty!");
        }
        return file;
    }
    catch (const std::exception& error) {
        //_Logger->addServerLog(_Logger->error, module_name + " " + std::string(error.what()), 2);
        exit(1);
    }
    catch (...) {
        //_Logger->addServerLog(_Logger->error, module_name + " catch unknw error", 2);
        exit(1);
    }
}

std::string Config::getPathByType(fileTypes type) {
    switch (type) {
    case Config::config:
        return "./Config.txt";
        break;
    case Config::env:
        return "./.env";
        break;
    default:
        throw std::runtime_error("unknw file type");
        break;
    }
}

void Config::parseFrom(fileTypes type) {
    try {
        std::string filename = getPathByType(type);
        std::ifstream file = openFile(filename);
        std::string line;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#' || line[0] == ' ' || line[0] == ';') { // empty line
                continue;
            }

            std::string key = line.substr(0, line.find("="));
            std::string value = line.substr(line.find("=") + 1);

            if (toLower(key).find("develop") != std::string::npos) {
                temp_vars_develop_[key.substr(0, toLower(key).find("_develop"))] = value;
                continue;
            }

            if (toLower(key).find("production") != std::string::npos) {
                temp_vars_production_[key.substr(0, toLower(key).find("_production"))] = value;
                continue;
            }

            if (line.find("=") == std::string::npos || value.empty()) { // key in config isnt init 
                throw std::runtime_error("Key: \"" + key + "\" isnt initialize. Please enter value like: \"" + key + "=value\"");
                //continue;
            }

            configuration_vars_[key] = value;
        }

        file.close();
    }
    catch (const std::exception& error) {
        //_Logger->addServerLog(_Logger->error, module_name + " " + std::string(error.what()), 2);
        //exit(1);
    }
    catch (...) {
        //_Logger->addServerLog(_Logger->error, module_name + " catch unknw error", 2);
        //exit(1);
    }
}

void Config::mergeVars() {
    try {
        std::string run_mode = configuration_vars_["mode"];

        if (toLower(run_mode) == "develop") {
            for (auto& [key, value] : temp_vars_develop_) {
                configuration_vars_[key] = value;
            }
        } 
        else if (toLower(run_mode) == "production") {
            for (auto& [key, value] : temp_vars_production_) {
                configuration_vars_[key] = value;
            }
        }

        temp_vars_develop_.clear();
        temp_vars_production_.clear();
    }
    catch (const std::exception& error) {
        //_Logger->addServerLog(_Logger->error, module_name + " " + std::string(error.what()), 2);
        exit(1);
    }
    catch (...) {
        //_Logger->addServerLog(_Logger->error, module_name + " catch unknw error", 2);
        exit(1);
    }
}

void Config::varsFillingCheck() {
    try {
        for (auto& [key, value] : configuration_vars_) {
            if (value == "null") {
                //throw std::runtime_error("Important Key: \"" + key + "\" isnt added in config. Please solve this problem");
                //std::cout << "Important Key: \"" + key + "\" isnt added in config. Please solve this problem" << std::endl;
            }
        }
    }
    catch (const std::exception& error) {
        //_Logger->addServerLog(_Logger->error, module_name + " " + std::string(error.what()), 2);
        exit(1);
    }
    catch (...) {
        //_Logger->addServerLog(_Logger->error, module_name + " catch unknw error", 2);
        exit(1);
    }
}

void Config::initialize() {
    parseFrom(config);
    parseFrom(env);
    mergeVars();
    varsFillingCheck();

    is_init_ = true;
}

bool Config::isInitialize() {
    return is_init_;
}
