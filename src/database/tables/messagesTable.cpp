#include "database/tables/messagesTable.h"

#include "database/database.h"

MessagesTable::MessagesTable() {
    _Database = std::make_shared<Database>();

    initStatements();
}

void MessagesTable::initStatements() {
    _PreparedStatementManager->registerStatement("addMessage", R"(INSERT INTO messages (chat_id, sender_uid, content, content_type) 
                                                                    VALUES ($1, $2, $3, $4) 
                                                                    RETURNING id;)");

    _PreparedStatementManager->registerStatement("updateIsPinned", R"(UPDATE messages
                                                                    SET is_pinned = TRUE
                                                                    WHERE id = $1
                                                                    AND chat_id = $2;)");

    _PreparedStatementManager->registerStatement("updateIsEdited", R"(UPDATE messages
                                                                    SET is_edited = TRUE
                                                                    WHERE id = $1
                                                                    AND chat_id = $2;)");

    _PreparedStatementManager->registerStatement("deleteMessage", R"(DELETE FROM messages
                                                                    WHERE id = $1
                                                                    AND chat_id = $2;)");

    _PreparedStatementManager->registerStatement("getMessageInfo", R"(SELECT
                                                                    id,
                                                                    chat_id,
                                                                    sender_uid,
                                                                    content,
                                                                    content_type
                                                                    FROM messages
                                                                    WHERE id = $1;)");
}

uint64_t MessagesTable::addMessage(uint64_t& UID, uint64_t& chat_id, std::string& content, std::string& content_type) {
    try {
        std::shared_ptr<pqxx::connection> _Connection = _Database->getConnection();
        pqxx::work transaction(*_Connection);

        pqxx::result txn_result = _PreparedStatementManager->exec(transaction, "addMessage", chat_id, UID, content, content_type);

        uint64_t message_id = txn_result[0][0].as<uint64_t>();

        transaction.commit();

        return message_id;
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
        return uint64_t();
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
        return uint64_t();
    }
}

void MessagesTable::updateIsPinned(uint64_t& chat_id, uint64_t& message_id) {
    try {
        std::shared_ptr<pqxx::connection> _Connection = _Database->getConnection();
        pqxx::work transaction(*_Connection);

        pqxx::result txn_result = _PreparedStatementManager->exec(transaction, "updateIsPinned", message_id, chat_id);

        transaction.commit();
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void MessagesTable::updateIsEdited(uint64_t& chat_id, uint64_t& message_id) {
    try {
        std::shared_ptr<pqxx::connection> _Connection = _Database->getConnection();
        pqxx::work transaction(*_Connection);

        pqxx::result txn_result = _PreparedStatementManager->exec(transaction, "updateIsEdited", message_id, chat_id);

        transaction.commit();
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void MessagesTable::deleteMessage(uint64_t& chat_id, uint64_t& message_id) {
    try {
        std::shared_ptr<pqxx::connection> _Connection = _Database->getConnection();
        pqxx::work transaction(*_Connection);

        pqxx::result txn_result = _PreparedStatementManager->exec(transaction, "deleteMessage", message_id, chat_id);

        transaction.commit();
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

std::map <std::string, std::string> MessagesTable::getMessageInfo(uint64_t& message_id) {
    try {
        std::map <std::string, std::string> message_info_map;
        std::shared_ptr<pqxx::connection> _Connection = _Database->getConnection();
        pqxx::work transaction(*_Connection);

        pqxx::result txn_result = _PreparedStatementManager->exec(transaction, "getMessageInfo", message_id);

        const pqxx::row& row = txn_result[0];
        for (pqxx::row::size_type i = 0; i < row.size(); ++i) {
            std::string column_name = txn_result.column_name(i);
            std::string value = row[i].c_str();

            message_info_map[column_name] = value;
        }

        transaction.commit();

        return message_info_map;
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
        return std::map <std::string, std::string>();
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
        return std::map <std::string, std::string>();
    }
}
