#ifndef USERSQUEUE_H
#define USERSQUEUE_H

#include <string>
#include <boost/asio.hpp>
#include <queue>

class RequestProcessing;

struct ConnectionInfoFields {
	std::string request;
	boost::asio::ip::tcp::socket& socket;
};

class UsersQueue {
private:
	std::shared_ptr<RequestProcessing> _RequestProcessing;
public:
	UsersQueue();
	~UsersQueue() = default;

	static UsersQueue* get_instance();

	void addUserToQueue(std::string request, boost::asio::ip::tcp::socket& socket);
	void queueHandler();
};

#endif 