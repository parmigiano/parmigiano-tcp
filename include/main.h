#ifndef MAIN_H
#define MAIN_H

#include "memory"

class CollectClientInfo;
class AutoUpdate;

class AppControl {
private:
	std::shared_ptr<CollectClientInfo> _CollectClientInfo;
	std::shared_ptr<AutoUpdate> _AutoUpdate;
public:
	AppControl();
	~AppControl() = default;

	void startApp();
};

//extern AppControl _AppControl;

#endif 
