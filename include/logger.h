#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
private:
	std::string getActualTime();
	int logFilesExist();
	int checkLogDirectoryExist();
	int fileLog(std::string logType, std::string log);
	int consoleLog(std::string logType, std::string log);
public:
	int initializeLogger();
	int addLog(std::string logType, std::string log, int loggingFlag); // types - info/error/problem ||| enteringFlag - 0: only file log, 1: file and console log
};

extern Logger _Logger;

#endif 