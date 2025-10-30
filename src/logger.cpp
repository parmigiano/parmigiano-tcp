#include "../include/logger.h"

#include "filesystem"
#include <chrono>

Logger* Logger::instance_ptr = nullptr;
std::mutex Logger::mtx;

Logger::Logger() {

}

Logger* Logger::get_instance(){
	std::lock_guard<std::mutex> lock(mtx);
	if (instance_ptr == nullptr) {
		instance_ptr = new Logger();
	}

	return instance_ptr;
}

std::string Logger::getActualTime(){
	std::string timeStr;

	auto now = std::chrono::system_clock::now();
	auto nowSeconds = std::chrono::floor<std::chrono::seconds>(now);

	timeStr = std::format("{:%H:%M:%S}", nowSeconds); // UTC time

	return timeStr;
}

int Logger::logFilesExist(){
	file.open(serverLogsDir + "\\latest.txt", std::ios::out);

	return 0;
}

int Logger::checkLogDirectoryExist(){
	if (!std::filesystem::exists(serverLogsDir)) {
		addServerLog(logType::info, "(logger) Created directory: " + serverLogsDir, 2);
		std::filesystem::create_directory(serverLogsDir);
	}

	return 0;
}

std::string Logger::definitionLogType(logType _logType){
	switch (_logType) {
	case Logger::info: return "INFO"; 
	case Logger::warn: return "WARN";
	case Logger::error: return "ERROR";
	default: return "UNKWN";
	}
}

int Logger::fileLog(logType _logType, std::string log){
	std::fstream latestLogFile;
	latestLogFile.open(serverLogsDir + "\\latest.txt", std::ios::app);

	latestLogFile << "[" << getActualTime() << "] " << definitionLogType(_logType) << " " << log << std::endl;

	latestLogFile.close();

	return 0;
}

int Logger::consoleLog(logType _logType, std::string log) {
	printf("[%s] %s %s\n", getActualTime().c_str(), definitionLogType(_logType).c_str(), log.c_str());

	return 0;
}

int Logger::initializeLogger(){
	checkLogDirectoryExist();
	logFilesExist();

	return 0;
}

void Logger::addServerLog(logType _logType, std::string log, unsigned short int loggingFlag){
	try {
		switch (loggingFlag) {
		case 0:
			fileLog(_logType, log);
			break;
		case 1:
			consoleLog(_logType, log);
			break;
		case 2:
			fileLog(_logType, log);
			consoleLog(_logType, log);
			break;
		default:
			addServerLog(logType::warn, "(logger) unkwn log type" + serverLogsDir, 2);
			break;
		}
	}
	catch (const std::exception& error) {
		addServerLog(Logger::warn, "(Logger) except: " + std::atoi(error.what()), 2);
	}
	catch (...) {
		addServerLog(Logger::warn, "(Logger) catch unknw error", 2);
	}
}

void Logger::addSessionLog(logType, std::string log, unsigned short int loggingFlag, std::string sessionID){

}
