#include "logger.h"

#include "filesystem"
#include <chrono>

Logger* Logger::instance_ptr_ = nullptr;
std::mutex Logger::mtx_;

Logger::Logger() {
	_Config = Config::get_instance();
}

Logger* Logger::get_instance(){
	std::lock_guard<std::mutex> lock(mtx_);
	if (instance_ptr_ == nullptr) {
		instance_ptr_ = new Logger();
	}

	return instance_ptr_;
}

std::string Logger::definitionLogType(logType log_type) {
	switch (log_type) {
	case Logger::info: return "INFO";
	case Logger::warn: return "WARN";
	case Logger::error: return "ERROR";
	default: return "UNKWN";
	}
}

std::string Logger::getActualTime() {
	std::string time_str;

	auto now = std::chrono::system_clock::now();
	auto now_seconds = std::chrono::floor<std::chrono::seconds>(now);

	time_str = std::format("{:%H:%M:%S}", now_seconds); // UTC time

	return time_str;
}

void Logger::fileReset(std::string filepath) {

}

void Logger::fileLog(logType log_type, std::string log, std::string filepath) {
	std::fstream latest_log_file;

	latest_log_file.open(filepath, std::ios::app);
	latest_log_file << "[" << getActualTime() << "] " << definitionLogType(log_type) << " " << log << std::endl;
	latest_log_file.close();
}

void Logger::consoleLog(logType log_type, std::string log) {
	printf("[%s] %s %s\n", getActualTime().c_str(), definitionLogType(log_type).c_str(), log.c_str());
}

void Logger::log(logType log_type, std::string& log_str, unsigned short int& logging_flag, std::string filepath) {
	switch (logging_flag) {
	case 0:
		fileLog(log_type, log_str, filepath);
		break;
	case 1:
		consoleLog(log_type, log_str);
		break;
	case 2:
		fileLog(log_type, log_str, filepath);
		consoleLog(log_type, log_str);
		break;
	default:
		break;
	}
}

void Logger::initialize() {
	try {
		checkDirExist(_Config->configuration_vars_["log_dir"] + "/" + _Config->configuration_vars_["log_server_subdir"]);
		checkDirExist(_Config->configuration_vars_["log_dir"] + "/" + _Config->configuration_vars_["log_session_subdir"]);

		fileReset(_Config->configuration_vars_["log_dir"] + "/" + _Config->configuration_vars_["log_server_subdir"] + "/" + "/latest.txt");

		is_init_ = true;
	}
	catch (const std::exception& error) {
		//addServerLog(Logger::warn, module_name + " except: " + std::string(error.what()), 2);
	}
	catch (...) {
		//addServerLog(Logger::warn, module_name + " catch unknw error", 2);
	}
}

inline bool Logger::isInitalized() const {
	return is_init_;
}

void Logger::checkDirExist(std::string filepath) {
	if (!std::filesystem::exists(filepath)) {
		std::filesystem::create_directories(filepath);
		addServerLog(logType::info, "(logger) Created directory: " + filepath, 2);
	}
}

void Logger::addServerLog(logType log_type, std::string log_str, unsigned short int logging_flag) {
	try {
		log(log_type, log_str, logging_flag, _Config->configuration_vars_["log_dir"] + "/" + _Config->configuration_vars_["log_server_subdir"] + "/latest.txt");
	}
	catch (const std::exception& error) {
		//addServerLog(Logger::warn, module_name + " except: " + std::string(error.what()), 2);
	}
	catch (...) {
		//addServerLog(Logger::warn, module_name + " catch unknw error", 2);
	}
}

void Logger::addSessionLog(logType log_type, std::string& UID, std::string log_str, unsigned short int logging_flag) {
	try {
		log(log_type, log_str, logging_flag, _Config->configuration_vars_["log_dir"] + "/" + _Config->configuration_vars_["log_session_subdir"] + UID + ".txt");
	}
	catch (const std::exception& error) {
		//addServerLog(Logger::warn, module_name + " except: " + std::string(error.what()), 2);
	}
	catch (...) {
		//addServerLog(Logger::warn, module_name + " catch unknw error", 2);
	}
}
