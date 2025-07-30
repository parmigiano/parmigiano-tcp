#include "../include/sendingRequests.h"
#include "../include/connection.h"
//#include "../include/serialization.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

//SendingRequests _SendingRequests;

//using json = nlohmann::json;

SendingRequests::SendingRequests(){
	_Connection = std::make_shared<Connection>();
	_Request = std::make_shared<RequestStruct::Request>();

	GOOGLE_PROTOBUF_VERIFY_VERSION;
}

SendingRequests::~SendingRequests(){
	google::protobuf::ShutdownProtobufLibrary();
}

void SendingRequests::sendRequest(){
	std::string data;

	if (!_Request->SerializeToString(&data)) {
		std::cerr << "Filed to serialize request"<< std::endl;
	}

	if (_Request->requestinfo().type() == RequestStruct::RequestInfo::unknown) {
		std::cerr << "Request will not sending with type requestInfo->requestType: unknown" << std::endl;
		return;
	}

	if (send(_Connection->getCurrentConnectSocket(), data.c_str(), (int)data.size(), 0) == SOCKET_ERROR) {
		std::cerr << "Send failed ith error: " << WSAGetLastError() << std::endl;
	}

	clearRequest();
}

int SendingRequests::clearRequest(){
	_Request->Clear();

	return 0;
}

int SendingRequests::addFileInfoToRequest(std::string fileName, std::string filePath, std::string fileHash){
	auto* fileInfo = _Request->add_filesinfo();

	fileInfo->set_name(fileName);
	fileInfo->set_path(filePath);

	if (!fileHash.empty()) {
		fileInfo->set_hash(fileHash);
	}

	return 0;
}

int SendingRequests::setRequestInfo(std::string requestType){
	if (requestType == "fileHashRequest") {
		_Request->mutable_requestinfo()->set_type(RequestStruct::RequestInfo::fileHashRequest);
	}
	else if (requestType == "downloadFile") {
		_Request->mutable_requestinfo()->set_type(RequestStruct::RequestInfo::downloadFile);
	}
	else {
		std::cerr << "incorrect request type" << std::endl;
		return 1;
	}

	return 0;
}

int SendingRequests::addDataToRequest(std::string key, std::string value){
	auto* requestData = _Request->add_requestdata();

	requestData->set_key(key);
	requestData->set_value(value);

	return 0;
}

int SendingRequests::confirmRequest(std::string requestType){
	/*if (!_Request->IsInitialized()) {
		std::cout << "(!) sended request with ONLY requestInfo->requestType: " << requestType << std::endl;
	}*/

	if (_Request->ByteSizeLong() == 0) {
		std::cout << "(!) sended request with ONLY requestInfo->requestType: " << requestType << std::endl;
	}

	//_Request->add_filesinfo()->set_hash("1");
	//_Request->add_filesinfo()->set_name("2");
	//_Request->add_filesinfo()->set_path("3");

	setRequestInfo(requestType);
	sendRequest();

	return 0;
}