#include "base.h"

Base::Base() {
	_Database = Database::get_instance();
	_Logger = Logger::get_instance();

	_PreparedStatementManager = PreparedStatementManager::get_instance();
}
