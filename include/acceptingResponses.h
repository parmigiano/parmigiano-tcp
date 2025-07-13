#ifndef ACCEPTINGRESPONSES_H
#define ACCEPTINGRESPONSES_H

#include <string>
#include <memory>

class Connection;

class AcceptingResponses {
private:
	std::shared_ptr<Connection> _Connection;
public:
	AcceptingResponses();
	~AcceptingResponses() = default;

	std::string acceptingServerResponseJson();
	int acceptingFiles(std::string filePath);
};

//extern AcceptingResponses _AcceptingResponses;

#endif 
