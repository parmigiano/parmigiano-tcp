#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../include/logger.h"
#include "../include/config.h"

#include <memory>
#include <functional>
#include <vector>
#include <chrono>
#include <thread>

class Task {
private:
	std::function<void()> func;
	std::chrono::seconds interval;
	std::chrono::time_point<std::chrono::system_clock> last_run;
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

	std::vector <std::shared_ptr<Task>> tasksList;
	bool running = false;

	void handleTasks();
public:
	Scheduler();
	~Scheduler() = default;

	void start();
	void stop();
	void addTask(std::shared_ptr<Task> task);
};

#endif 