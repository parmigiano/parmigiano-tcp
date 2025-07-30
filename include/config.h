#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <mutex>

struct Config {
private:
    static Config* instance_ptr;
    static std::mutex mtx;

    Config(){}
public:
    Config(const Config&) = delete;
    ~Config();

    static Config* get_instance();
    
    // Fields for update
    std::string nameOfMainExeFile;

    std::string appVersion;
    std::string UID;
    std::string region;
};

#endif 