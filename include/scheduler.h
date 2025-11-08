#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "logger.h"
#include "config.h"

#include <memory>
#include <functional>
#include <vector>
#include <chrono>
#include <thread>

class Task {
private:
	std::function<void()> func_;
	std::chrono::seconds interval_;
	std::chrono::time_point<std::chrono::system_clock> last_run_;
public:
	Task(std::function<void()> f, int interval_minutes);
	~Task() = default;

	bool shouldRun();
	void run();
};

class Scheduler {
private:
	Config* _Config;
	Logger* _Logger;

	std::vector<std::shared_ptr<Task>> tasks_list_;

	void handleTasks();

	bool running_ = false;
	const std::string MODULE_NAME_ = "(Scheduler)";
public:
	Scheduler();
	~Scheduler() = default;

	void start();
	void stop();
	void addTask(std::shared_ptr<Task> task);
};

#endif 