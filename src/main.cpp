#include "../include/main.h"
#include "../include/collectClientInfo.h"
#include "../include/autoUpdate.h"

//#include "../include/sendingRequests.h" // temp

#include <iostream>

//AppControl _AppControl;

int main() {
	setlocale(LC_ALL, "ru");
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
	//CollectClientInfo _collectClientInfo;
	//AutoUpdate _autoUpdate;

	_CollectClientInfo->setClientAppInfo("1.0.0", "unknown");
	short int resultOfCheckingUpdate = _AutoUpdate->checkUpdate();

	if (resultOfCheckingUpdate == 1) //  if 1 - the client version does not match the latest build version
		_AutoUpdate->downloadUpdate();
}
