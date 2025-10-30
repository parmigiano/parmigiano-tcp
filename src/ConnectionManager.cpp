#include "../include/ConnectionManager.h"

#include <random>

ConnectionManager* ConnectionManager::instance_ptr = nullptr;
std::mutex ConnectionManager::mtx;

ConnectionManager::ConnectionManager() {

}

ConnectionManager* ConnectionManager::get_instance() {
	std::lock_guard<std::mutex> lock(mtx);
	if (instance_ptr == nullptr) {
		instance_ptr = new ConnectionManager();
	}

	return instance_ptr;
}

std::string ConnectionManager::generateUUID() {
	static std::random_device dev;
	static std::mt19937 rng(dev());
	std::uniform_int_distribution<int> dist(0, 15);

	const char* symbols = "0123456789abcdef";
	std::vector <int> UUIDpattern = { 8, 4, 4, 4, 12 };
	std::string UUID;

	for (auto& element : UUIDpattern) {
		for (int i = 0; i < element; i++) {
			UUID += symbols[dist(rng)];
		}
		if (&element == &UUIDpattern.back()) continue;
		UUID.append("-");
	}

	return UUID;
}

void ConnectionManager::assigningSession(boost::asio::ip::tcp::socket& socket, std::string UUID, std::string cryptKey) {
	// auto it = _UsersIdentify.emplace(UUID, std::make_pair(std::move(socket), cryptKey));

	auto it = _UsersIdentify.find(UUID);

	if (it != _UsersIdentify.end()) {
		it->second = std::make_pair(std::move(socket), cryptKey);
	}
	else {
		_UsersIdentify.emplace(UUID, std::make_pair(std::move(socket), cryptKey));
	}
}

void ConnectionManager::replaceSocketByUUID(boost::asio::ip::tcp::socket& socket, std::string UUID, std::string cryptKey) {
	//_UsersIdentify[UUID] = std::make_pair(std::move(socket), cryptKey);
}