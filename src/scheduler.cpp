#include "../include/scheduler.h"

// TASK CLASS
Task::Task(std::function<void()> f, int interval_minutes) : func(f), interval(std::chrono::seconds(interval_minutes)), last_run(std::chrono::system_clock::now()) { // change seconds to minutes
}

bool Task::shouldRun() {
    auto now = std::chrono::system_clock::now();

    if (now - last_run >= interval) {
        last_run = now;
        return true;
    }

    return false;
}

void Task::run() {
    std::thread(func).detach();
}

// SCHEDULER CLASS
Scheduler::Scheduler() {
	_Logger = Logger::get_instance();
	_Config = Config::get_instance();
}

void Scheduler::start() {
    _Logger->addServerLog(_Logger->info, "(scheduler) start", 2);
    std::thread(&Scheduler::handleTasks, this).detach();
}

void Scheduler::stop() {
    running = false;
}

void Scheduler::handleTasks() {
    try {
        running = true;

        while (running) {
            for (auto& task : tasksList) {
                if (task->shouldRun()) {
                    task->run();
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, "(scheduler) except: " + std::atoi(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, "(scheduler) catch unknw error", 2);
    }
}

void Scheduler::addTask(std::shared_ptr<Task> task) {
    try {
        _Logger->addServerLog(_Logger->info, "(scheduler) added new task", 2);

        tasksList.push_back(task);
    }
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, "(scheduler) except: " + std::atoi(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, "(scheduler) catch unknw error", 2);
    }
}