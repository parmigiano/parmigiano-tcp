#ifndef AUTOUPDATE_H
#define AUTOUPDATE_H

//#include "../json-develop/single_include/nlohmann/json.hpp"
//#include "../include/RequestStruct.pb.h"
#include "../include/ResponseStruct.pb.h"

#include <string>
//#include <filesystem> 
#include <map>
#include <filesystem>
#include <memory>

class AcceptingResponses;
class SendingRequests;
class Connection;

class AutoUpdate {
private:
	std::shared_ptr<AcceptingResponses> _AcceptingResponses;
	std::shared_ptr<SendingRequests> _SendingRequests;
	std::shared_ptr<Connection> _Connection;
	std::shared_ptr<ResponseStruct::Response> _Response;
	//std::shared_ptr<RequestStruct::Request> _Request;

	std::string getHashLatestBuildFromServer();
	// RequestStruct::Request getHashFromCurrentClientFiles(RequestStruct::Request actualFilesInfo);

	std::size_t hash_file(const std::string& filePath);

	int downloadFiles();
	int startUpdater();
	int directoriesExist(std::shared_ptr<ResponseStruct::Response> actualFilesInfo);
	int comparisonHash(std::shared_ptr<ResponseStruct::Response> actualFilesInfo);

public:
	AutoUpdate();
	~AutoUpdate();

	int checkUpdate();
	int downloadUpdate();
};

//extern AutoUpdate _AutoUpdate;

#endif 
