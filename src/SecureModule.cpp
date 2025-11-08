#include "SecureModule.h"

#include "sendResponse.h"

SecureModule::SecureModule() {
	_Logger = Logger::get_instance();
	_Config = Config::get_instance();
}