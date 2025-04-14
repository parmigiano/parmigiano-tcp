#include "../include/sendResponse.h"
#include "../include/connectionHandler.h"

int SendResponse::sendFile(){
	return 0;
}

int SendResponse::sendJSON(json j){
	ConnectionHandler _ConnectionHandler;

	if (j.empty()) {
		cout << "(!) json is empty. Sending json response is stopped." << std::endl;
		return 1;
	}

	j["responseInfo"]["responseType"] = "actualClientFilesHashes";

	send(_ConnectionHandler.getCurrentConnectClientSocket(), j.dump().c_str(), (int)j.dump().size(), 0);
	return 0;
}
