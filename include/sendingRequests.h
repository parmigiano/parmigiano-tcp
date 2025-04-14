#ifndef SENDINGREQUESTS_H
#define SENDINGREQUESTS_H

#include <string>
#include <map>

class SendingRequests {
private:
	void sendRequest();
public:
	int clearRequest();
	int addParameter(std::string parameterCaseName, std::string parameterKey, std::string parameterMeaning);
	int addParameter(std::string parameterCaseName, std::map <std::string, std::string> requestData);
	int confirmRequest(std::string requestType);
	//void sendSystemInfo();
};

#endif 
