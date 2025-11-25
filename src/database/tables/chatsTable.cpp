#include "database/tables/chatsTable.h"

#include "database/database.h"

ChatsTable::ChatsTable() {
	_Database = std::make_shared<Database>();

}

void ChatsTable::initStatements() {
}
