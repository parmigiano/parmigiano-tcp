#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <string>
#include <unordered_map>

class Serialization {
private:
public:
	std::string requestStructToJson(std::unordered_map<std::string, std::unordered_map<std::string, std::string>> requestData);
};

#endif 
