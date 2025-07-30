#ifndef COLLECTCLIENTINFO_H
#define COLLECTCLIENTINFO_H

#include <string>

class CollectClientInfo {
private:
	std::string m_OSversion;
	std::string m_region;  // system information

	void getOSversion();
	void getRegion();
public:
	~CollectClientInfo() = default;

	void collectSystemInfo();
};

//extern CollectClientInfo _CollectClientInfo;

#endif 
