#ifndef LOGGER_H
#define LOGGER_H

#include <config.h>

#include <string>
#include <fstream>
#include <mutex>

class Logger {
public:
	enum logType { 
		info, 
		warn, 
		error 
	};
private:
	Config* _Config;

	static Logger* instance_ptr_;
	static std::mutex mtx_;
	std::ofstream file_;

	std::string definitionLogType(logType);
	std::string getActualTime();

	void fileReset(std::string filepath);
	void fileLog(logType, std::string log_str, std::string filepath);
	void consoleLog(logType, std::string log_str);
	void log(logType log_type, std::string& log_str, unsigned short int& logging_flag, std::string filepath);

	void checkDirExist(std::string filepath);

	const std::string MODULE_NAME_ = "(Logger)";
	bool is_init_ = false;
public:
	Logger();
	Logger(const Logger&) = delete;
	~Logger() = default;

	static Logger* get_instance();

	void initialize();
	inline bool isInitalized() const;

	void addServerLog(logType, std::string log_str, unsigned short int logging_flag);
	void addSessionLog(logType, std::string& UID, std::string log_str, unsigned short int logging_flag);
};

#endif 