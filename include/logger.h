#ifndef LOGGER_H
#define LOGGER_H

#include <string>

#include "../include/config.h"

//class Config;

class Logger {
private:
	static Logger* instance_ptr;
	static std::mutex mtx;

	Config* _Config;

	std::string getActualTime();
	int logFilesExist();
	int checkLogDirectoryExist();
	int fileLog(std::string logType, std::string log);
	int consoleLog(std::string logType, std::string log);
public:
	Logger();

	Logger(const Logger&) = delete;
	~Logger() = default;

	static Logger* get_instance();

	int initializeLogger();
	int addLog(std::string logType, std::string log, int loggingFlag); // types - info/error/problem ||| enteringFlag - 0: only file log, 1: only console log 2: file and console log
};

//extern Logger _Logger;

#endif 