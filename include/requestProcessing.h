#ifndef REQUESTPROCESSING_H
#define REQUESTPROCESSING_H

#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

class requestProcessing {
private:
public:
	int requestDistribution(std::string request);
};

#endif 