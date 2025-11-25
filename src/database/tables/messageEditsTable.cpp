#include "database/tables/messageEditsTable.h"

#include "database/database.h"

MessageEditsTable::MessageEditsTable() {
	_Database = std::make_shared<Database>();

}
