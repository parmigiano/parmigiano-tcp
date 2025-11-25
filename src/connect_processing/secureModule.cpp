#include "connect_processing/secureModule.h"

#include "connect_processing/sendResponse.h"

SecureModule::SecureModule() {
	_Logger = Logger::get_instance();
	_Config = Config::get_instance();
}