#include "../include/main.h"
#include "../include/collectClientInfo.h"
#include "../include/autoUpdate.h"

#include "../include/sendingRequests.h" // temp

#include <iostream>

using namespace std;

int main() {
	setlocale(LC_ALL, "ru");
	AppControl _appControl;

	_appControl.startApp();

	system("pause");

	return 0;
}

void AppControl::startApp() {
	CollectClientInfo _collectClientInfo;
	AutoUpdate _autoUpdate;

	_collectClientInfo.setClientAppInfo("1.0.0", "unknown");
	short int resultOfCheckingUpdate = _autoUpdate.checkUpdate();

	if (resultOfCheckingUpdate == 1) //  if 1 - the client version does not match the latest build version
		_autoUpdate.downloadUpdate();

	//sendingRequests _sendingRequests;

	//for(int i = 0; i < 3; i++)
	//	_SendingRequests.addParameter("user_info", "name", "Oleg"); // temp

	//_sendingRequests.confirmRequest("send_info_about_user");
}
