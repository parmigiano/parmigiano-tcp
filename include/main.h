#ifndef MAIN_H
#define MAIN_H

#include "memory"

class ProcessingFile;

class AppControl {
private:
	int argc;
	char** argv;

	std::shared_ptr<ProcessingFile> _ProcessingFile;
public:
	AppControl(int argc, char** argv);
	~AppControl() = default;

	void startApp();
};

#endif 
