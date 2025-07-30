#include "../include/config.h"

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