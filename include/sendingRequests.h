#ifndef SENDINGREQUESTS_H
#define SENDINGREQUESTS_H

#include <string>
#include <map>
#include <memory>

#include "../include/RequestStruct.pb.h"

class Connection;

class SendingRequests {
private:
	void sendRequest();

	std::shared_ptr<Connection> _Connection;
	std::shared_ptr<RequestStruct::Request> _Request;

public:
	SendingRequests();
	~SendingRequests();

	int clearRequest();
	int addFileInfoToRequest(std::string fileName, std::string filePath, std::string fileHash = "");
	int setRequestInfo(std::string requestType);
	int addDataToRequest(std::string key, std::string value);
	int confirmRequest(std::string requestType);
	//void sendSystemInfo();
};

//extern SendingRequests _SendingRequests;

#endif 