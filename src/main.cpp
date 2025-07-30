#include "../include/main.h"
#include "../include/collectClientInfo.h"
#include "../include/autoUpdate.h"
#include "../include/config.h"

//#include "../include/sendingRequests.h" // temp

#include <iostream>
#include "windows.h" 

//AppControl _AppControl;

int main(int argc, char** argv) {
	setlocale(LC_ALL, "ru");

	const char* exename = strrchr(argv[0], '\\');
	if (exename)
		++exename;
	else
		exename = argv[0];

	std::string exenameStr("");
	exenameStr.append(exename);

	Config::get_instance()->nameOfMainExeFile = exenameStr;

	AppControl* _AppControl = new AppControl();
	_AppControl->startApp();
	delete _AppControl; 

	system("pause");

	return 0;

}

AppControl::AppControl(){
	_CollectClientInfo = std::make_shared<CollectClientInfo>();
	_AutoUpdate = std::make_shared<AutoUpdate>();
}

void AppControl::startApp() {
	Config::get_instance()->appVersion = "0.0.1";
	Config::get_instance()->UID = "unknown";

	short int resultOfCheckingUpdate = _AutoUpdate->checkUpdate();

	if (resultOfCheckingUpdate == 1) //  if 1 - the client version does not match the latest build version
		_AutoUpdate->downloadUpdate();
}
