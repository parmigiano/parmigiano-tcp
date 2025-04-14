#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

class ConnectionHandler {
private:
public:
	//SOCKET getCurrentSocket() const;
	void addUserToQueue(string request, SOCKET socket);
	void queueHandler();
	void incomingConnections();
	SOCKET getCurrentConnectClientSocket();
	//ConnectionHandler();
};

#endif 