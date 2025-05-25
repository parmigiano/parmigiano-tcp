#ifndef ACCEPTINGRESPONSES_H
#define ACCEPTINGRESPONSES_H

#include <string>

class AcceptingResponses {
private:
public:
	std::string acceptingServerResponseJson();
	int acceptingFiles(std::string filePath);
};

#endif 
