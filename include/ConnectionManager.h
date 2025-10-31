#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <string>
#include <vector>
#include <mutex>
#include <map>
#include <boost/asio.hpp>

class ConnectionManager {
private:
	static ConnectionManager* instance_ptr;
	static std::mutex mtx;

	typedef std::pair<boost::asio::ip::tcp::socket, std::string> key;
	std::map <std::string, std::pair<boost::asio::ip::tcp::socket, std::string>> _UsersIdentify;
public:
	ConnectionManager();
	ConnectionManager(const ConnectionManager&) = delete;
	~ConnectionManager() = default;

	static ConnectionManager* get_instance();

	std::string generateUUID();
	void assigningSession(boost::asio::ip::tcp::socket& socket, std::string UUID, std::string cryptKey);
	void replaceSocketByUUID(boost::asio::ip::tcp::socket& socket, std::string UUID, std::string cryptKey);
};

#endif 