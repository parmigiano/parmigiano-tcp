#include "../include/requestProcessing.h"

#include "../include/sendResponse.h"

#include <regex>

RequestProcessing::RequestProcessing(){
    _Logger = Logger::get_instance();
    _Config = Config::get_instance();
    
    _SendResponse = std::make_shared<SendResponse>();
}

int RequestProcessing::requestDistribution(std::string requestStr, boost::asio::ip::tcp::socket& socket){
    ClientRequestStruct::Request acceptedRequest;
    acceptedRequest.ParseFromString(requestStr);

    try {
        switch (acceptedRequest.requestinfo().type()) {
        default:
            _Logger->addServerLog(_Logger->warn, "(requestProcessing) unknw request type", 2);
            break;
        }
    } 
    catch (const std::exception& error) {
        _Logger->addServerLog(_Logger->warn, "(requestProcessing) except: " + std::atoi(error.what()), 2);
    }
    catch (...) {
        _Logger->addServerLog(_Logger->warn, "(requestProcessing) catch unknw error", 2);
    }
    
    return 0;
}
