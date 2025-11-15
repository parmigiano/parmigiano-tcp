#include "database/tables/baseTable.h"

BaseTable::BaseTable() {
	_Database = Database::get_instance();
	_Logger = Logger::get_instance();
	_PreparedStatementManager = PreparedStatementManager::get_instance();

	_Connection = _Database->getConnection();
}
