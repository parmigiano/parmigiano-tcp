#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <iostream>
#include <map>
#include <list>
#include <mutex>

class FileInfoFields {
public:
    std::string fileName;
    std::string filePath;
    std::string fileHash;

    FileInfoFields(std::string name, std::string path, std::string hash) : fileName(name), filePath(path), fileHash(hash) {};
};

typedef std::list<FileInfoFields> _FileInfoFields;

struct Config {
private:
    static Config* instance_ptr;
    static std::mutex mtx;

    Config(){}

    int isFile();
    void assigningValues(std::string key, std::string value);
public:
    Config(const Config&) = delete;
    ~Config();

    static Config* get_instance();

    short int serverPort = 0;
    std::string buildDir = "";
    std::string logDir = "";
    //nlohmann::json latestBuildInfo;

    _FileInfoFields buildFilesInfo;
    std::list <std::string> buildDirsInfo;

    int parseConfig();
};

#endif 