#ifndef CUSTOMLIST_H
#define CUSTOMLIST_H

#include <string>
#include <map>
#include <list>

class RequestStructure {
public:
    std::list <std::map <std::string, std::list<std::map <std::string, std::string>>>> requestData;

    RequestStructure(std::list <std::map <std::string, std::list<std::map <std::string, std::string>>>> requestData);
};

typedef std::list<RequestStructure> reqStruct;
extern reqStruct requestData;

#endif 
