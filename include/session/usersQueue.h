#pragma once

//#include <session/session.h>
#include <string>
#include <boost/asio.hpp>
#include <queue>
#include <mutex>
#include <condition_variable>

class RequestProcessing;
class Session; // forward declaration

struct ConnectionInfoFields {
	std::string request;
	//boost::asio::ip::tcp::socket& socket;
	Session& session;
};

class UsersQueue {
private:
	std::shared_ptr<RequestProcessing> _RequestProcessing;

	static UsersQueue* instance_ptr_;
	static std::mutex instance_mtx_;
public:
	UsersQueue();
	~UsersQueue() = default;

	static UsersQueue* get_instance();

	void addUserToQueue(std::string request, Session& session);
	void queueHandler();
};