#include "database/preparedStatementManager.h"

PreparedStatementManager* PreparedStatementManager::instance_ptr_ = nullptr;
std::mutex PreparedStatementManager::instance_mtx_;

PreparedStatementManager::PreparedStatementManager() {
    _Database = Database::get_instance();
    _Logger = Logger::get_instance();

    conn_ = _Database->getConnection();
}

PreparedStatementManager* PreparedStatementManager::get_instance() {
    std::lock_guard<std::mutex> lock(instance_mtx_);
    if (instance_ptr_ == nullptr) {
        instance_ptr_ = new PreparedStatementManager();
    }

    return instance_ptr_;
}

void PreparedStatementManager::registerStatement(const std::string& name, const std::string& query) {
    try {
        if (!isRegistered(name)) {
            prepared_[name] = query;
            conn_->prepare(name, query);
            _Logger->addServerLog(_Logger->info, MODULE_NAME_ + " Registered: " + name, 2);
        }
        else {
            _Logger->addServerLog(_Logger->info, MODULE_NAME_ + "Statement: \"" + name + "\" alredy exist", 2);
        }
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " unknown exception", 2);
    }
}

bool PreparedStatementManager::isRegistered(const std::string& name) {
    return prepared_.count(name) > 0;
}

void PreparedStatementManager::reprepareAll() {
    try {
        for (auto& [name, query] : prepared_) {
            conn_->prepare(name, query);
            _Logger->addServerLog(_Logger->info, MODULE_NAME_ + " Reprepared: " + name, 2);
        }
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " unknown exception", 2);
    }
}

std::string PreparedStatementManager::getSQL(const std::string& name) {
    try {
        auto it = prepared_.find(name);
        return (it != prepared_.end()) ? it->second : "";
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + "(PreparedStatementManager) except: " + std::string(error.what()), 2);
        return "";
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + "(PreparedStatementManager) unknown exception", 2);
        return "";
    }
}
