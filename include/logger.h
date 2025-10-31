#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

class Logger {
public:
	enum logType { info, warn, error };
	
private:
	static Logger* instance_ptr;
	static std::mutex mtx;

	std::ofstream file;

	// logger configuration
	std::string serverLogsDir = "./log";

	std::string getActualTime();
	int logFilesExist();
	int checkLogDirectoryExist();
	std::string definitionLogType(logType);
	int fileLog(logType, std::string log);
	int consoleLog(logType, std::string log);
public:
	Logger();
	Logger(const Logger&) = delete;
	~Logger() = default;

	static Logger* get_instance();

	int initializeLogger();
	void addServerLog(logType, std::string log, unsigned short int loggingFlag); // types - info/warn/error ||| 0: only file log | 1: only console log | 2: file and console log
	void addSessionLog(logType, std::string log, unsigned short int loggingFlag, std::string sessionID); // sessionID for unique file name. Logs with session is store in other dir
};

#endif
