#include "connect_processing/user_actions/userActionBase.h"

//#include "connect_processing/sendResponse.h"
//#include "session/sessionManager.h"
//#include "database/tables/chatMembersTable.h"
//#include "database/tables/messagesTable.h"
//#include "database/tables/userActivesTable.h"

UserActionBase::UserActionBase() {
    _SessionManager = SessionManager::get_instance();
    _Logger = Logger::get_instance();

    _SendResponse = std::make_shared<SendResponse>();
    //_ChatMembersTable = std::make_shared<ChatMembersTable>();
    //_MessagesTable = std::make_shared<MessagesTable>();
    //_UserActivesTable = std::make_shared<UserActivesTable>();
}
