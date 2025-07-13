#ifndef COLLECTCLIENTINFO_H
#define COLLECTCLIENTINFO_H

#include <string>

class CollectClientInfo {
private:
	std::string m_OSversion;
	std::string m_region;  // system information

	std::string m_appVersion;
	std::string m_UID;  // client application information

	void getOSversion();
	void getRegion();
public:
	~CollectClientInfo() = default;

	void collectSystemInfo();
	void setClientAppInfo(std::string _appVersion, std::string _UID);
};

//extern CollectClientInfo _CollectClientInfo;

#endif 
