#include "../include/main.h"
#include "../include/processingFile.h"

#include <iostream>
#include "windows.h" 

int main(int argc, char** argv) {
	setlocale(LC_ALL, "ru");

	AppControl* _AppControl = new AppControl(argc, argv);
	_AppControl->startApp();
	delete _AppControl;

	system("pause");

	return 0;
}

AppControl::AppControl(int argc, char** argv) : argc(argc), argv(argv) {
	_ProcessingFile = std::make_shared<ProcessingFile>();
}

void AppControl::startApp() {
	_ProcessingFile->replaceTempToMain(argc, argv);
}
