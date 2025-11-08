#include "scheduler.h"

// TASK CLASS
Task::Task(std::function<void()> f, int interval_minutes) : func_(f), interval_(std::chrono::seconds(interval_minutes)), last_run_(std::chrono::system_clock::now()) { // change seconds to minutes
}

bool Task::shouldRun() {
    auto now = std::chrono::system_clock::now();

    if (now - last_run_ >= interval_) {
        last_run_ = now;
        return true;
    }

    return false;
}

void Task::run() {
    std::thread(func_).detach();
}

// SCHEDULER CLASS
Scheduler::Scheduler() {
	_Logger = Logger::get_instance();
	_Config = Config::get_instance();
}

void Scheduler::start() {
    _Logger->addServerLog(_Logger->info, MODULE_NAME_ + " Started", 2);
    std::thread(&Scheduler::handleTasks, this).detach();
}

void Scheduler::stop() {
    running_ = false;
}

void Scheduler::handleTasks() {
    try {
        running_ = true;

        while (running_) {
            for (auto& task : tasks_list_) {
                if (task->shouldRun()) {
                    task->run();
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}

void Scheduler::addTask(std::shared_ptr<Task> task) {
    try {
        _Logger->addServerLog(_Logger->info, MODULE_NAME_ + " added new task", 2);

        tasks_list_.push_back(task);
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " except: " + std::string(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " catch unknw error", 2);
    }
}