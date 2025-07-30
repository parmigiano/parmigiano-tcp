#include "../include/logger.h"

#include "filesystem"
#include "fstream"
#include <chrono>

Logger* Logger::instance_ptr = nullptr;
std::mutex Logger::mtx;

//Logger _Logger;

Logger::Logger(){
	_Config = Config::get_instance();
}

Logger* Logger::get_instance(){
	std::lock_guard<std::mutex> lock(mtx);
	if (instance_ptr == nullptr) {
		instance_ptr = new Logger();
	}

	return instance_ptr;
}

std::string Logger::getActualTime(){
	std::ostringstream oss;

	auto now = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()); // in UTC format

	auto time_today = now - std::chrono::floor<std::chrono::days>(now);

	auto h = std::chrono::duration_cast<std::chrono::hours>(time_today);
	auto m = std::chrono::duration_cast<std::chrono::minutes>(time_today - h);
	auto s = std::chrono::duration_cast<std::chrono::seconds>(time_today - h - m);

	oss << std::setfill('0') << std::setw(2) << h.count() << ':' << std::setw(2) << m.count() << ':' << std::setw(2) << s.count();    

	return oss.str() + "UTC";
}

int Logger::logFilesExist(){
	std::ofstream file;

	file.open(_Config->logDir + "\\latest.txt", std::ios::out);

	file.close();
	return 0;
}

int Logger::checkLogDirectoryExist(){
	if (!std::filesystem::exists(_Config->logDir)) {
		// std::cout << "Created directory: " << _Config.logDir << std::endl;
		addLog("INFO", "(logger) Created directory: " + _Config->logDir, 2);
		std::filesystem::create_directory(_Config->logDir);
	}

	return 0;
}

int Logger::fileLog(std::string logType, std::string log){
	std::fstream file;

	file.open(_Config->logDir + "\\latest.txt", std::ios::app);

	file << "[" << logType << "] " << getActualTime() << " " << log << std::endl;

	file.close();

	return 0;
}

int Logger::consoleLog(std::string logType, std::string log){
	printf("[%s] %s %s\n", logType.c_str(), getActualTime().c_str(), log.c_str());

	return 0;
}

int Logger::initializeLogger(){
	checkLogDirectoryExist();
	logFilesExist();

	return 0;
}

int Logger::addLog(std::string logType, std::string log, int loggingFlag){
	if (loggingFlag == 0) {
		fileLog(logType, log);
	}

	if (loggingFlag == 1) {
		consoleLog(logType, log);
	}

	if (loggingFlag == 2) {
		fileLog(logType, log);
		consoleLog(logType, log);
	}

	return 0;
}