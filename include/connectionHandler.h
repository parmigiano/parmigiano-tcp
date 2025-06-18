#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <memory>

#include "../include/logger.h"
//#include "../include/connect.h"
//#include "../include/requestProcessing.h"

class RequestProcessing;
class Connection;

//using namespace std;

class ConnectionHandler {
private:
	Logger* _Logger;

	/*RequestProcessing* _RequestProcessing = nullptr;
	Connection* _Connection = nullptr;*/

	std::shared_ptr<RequestProcessing> _RequestProcessing;
	std::shared_ptr<Connection> _Connection;

public:
	ConnectionHandler();
	~ConnectionHandler() = default;

	//SOCKET getCurrentSocket() const;
	void addUserToQueue(std::string request, SOCKET socket);
	void queueHandler();
	void incomingConnections();
	SOCKET getCurrentConnectClientSocket();
	//ConnectionHandler();
};

//extern ConnectionHandler _ConnectionHandler;

#endif 