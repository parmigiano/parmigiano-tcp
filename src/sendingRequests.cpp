#include "../include/sendingRequests.h"
#include "../include/connection.h"
//#include "../include/serialization.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#include "../json-develop/single_include/nlohmann/json.hpp"

using json = nlohmann::json;
json j;

void SendingRequests::sendRequest(){
	Connection _connection;
	//Serialization _serialization;

	SOCKET connectSocket = _connection.getCurrentConnectSocket();

	// std::cout << _serialization.requestStructToJson(requestData) << std::endl;
	
	send(connectSocket, j.dump().c_str(), (int)j.dump().size(), 0);
}

int SendingRequests::clearRequest(){
	j.clear();
	return 0;
}

int SendingRequests::addParameter(std::string parameterCaseName, std::string parameterKey, std::string parameterValue){

	/*if (!j.contains(parameterCaseName)) {
		std::cout << "(!) Case not found. Case is created. Data is added to case" << std::endl;
		j[parameterCaseName][parameterKey] = parameterValue;
	}*/
	if (!parameterCaseName.empty() && !parameterKey.empty() && !parameterValue.empty()) {
		j[parameterCaseName][parameterKey] = parameterValue;
	}

	/*if (requestData.find(parameterCaseName) == requestData.end()) {
		std::cout << "(!) Case not found. Case is created. Data is added to case" << std::endl;
		requestData[parameterCaseName][parameterKey] = parameterValue;
	} else {

		if (requestData[parameterCaseName].find(parameterKey) == requestData[parameterCaseName].end()) {
			requestData[parameterCaseName][parameterKey] = parameterValue;
		} else {
			requestData[parameterCaseName][parameterKey].insert(requestData[parameterCaseName][parameterKey].end(), parameterValue.begin(), parameterValue.end());
		}

	}*/

	/*for (const auto& outerPair : requestData) {
		std::cout << "Outer Key: " << outerPair.first << std::endl;
		for (const auto& innerPair : outerPair.second) {
			std::cout << "  Inner Key: " << innerPair.first << ", Value: " << innerPair.second << std::endl;
		}
	}*/

	return 0;
}

int SendingRequests::addParameter(std::string parameterCaseName, std::map<std::string, std::string> transmittedRequestData){

	for (const auto& a : transmittedRequestData) {
		addParameter(parameterCaseName, a.first, a.second);
	}

	return 0;
}

int SendingRequests::confirmRequest(std::string requestType){
	if (j.empty()) {
		std::cout << "(!) sended request with ONLY requestInfo:requestType!" << std::endl;
	}

	addParameter("requestInfo", "requestType", requestType);
	sendRequest();

	return 0;
}