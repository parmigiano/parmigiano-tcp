#include "database/tables/baseTable.h"

//#include "database/database.h"

BaseTable::BaseTable() {
	//_Database = Database::get_instance();
	_Logger = Logger::get_instance();
	_PreparedStatementManager = PreparedStatementManager::get_instance();

	//_Database = std::make_shared<Database>();
	//_Connection = _Database->getConnection();
}
