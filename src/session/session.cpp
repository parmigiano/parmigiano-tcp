#include "session/session.h"

#include "session/usersQueue.h"
#include "connect_processing/userStatusService.h"

#include <iostream>

Session::Session(boost::asio::io_context& io_context) : client_socket_(io_context) {
    _Logger = Logger::get_instance();
    _Config = Config::get_instance();

    _Config->write_handler_ptr = [this] (const boost::system::error_code& error, size_t bytes) {
        this->handleWrite(error, bytes);
    };

    _UsersQueue = std::make_shared<UsersQueue>();
    _UserStatusService = std::make_shared<UserStatusService>();

}

void Session::handleDisconnect(const boost::system::error_code& error) {
    if (error == boost::asio::error::eof) {
        _Logger->addServerLog(_Logger->info, MODULE_NAME_ + " Client disconnected gracefully", 2);
    }
    else if (error == boost::asio::error::connection_reset) {
        _Logger->addServerLog(_Logger->info, MODULE_NAME_ + " Client connection reset", 2);
    }
    else {
        _Logger->addServerLog(_Logger->warn, MODULE_NAME_ + " Client connection error: " + error.message(), 2);
    }

    _UserStatusService->notifyOffline(socket());

    boost::system::error_code ignore_ec;
    client_socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignore_ec);
    client_socket_.close(ignore_ec);
}

void Session::readHeader(const boost::system::error_code& error, size_t bytes) {
    if (!error) {
        msg_length_ = ntohl(msg_length_);
        msg_data_.resize(msg_length_);

        boost::asio::async_read(client_socket_, boost::asio::buffer(msg_data_.data(), msg_length_),[self = shared_from_this()] (const boost::system::error_code& error, std::size_t bytes_transferred) {
                self->readBody(error, bytes_transferred);
            });
    }
    else {
        handleDisconnect(error);
    }
}

void Session::readBody(const boost::system::error_code& error, size_t bytes) {
    if (!error) {
        std::string data(msg_data_.begin(), msg_data_.end());

        _UsersQueue->addUserToQueue(data, client_socket_);

        boost::asio::async_read(client_socket_, boost::asio::buffer(&msg_length_, sizeof(msg_length_)),[self = shared_from_this()] (const boost::system::error_code& error, std::size_t bytes_transferred) {
                self->readHeader(error, bytes_transferred);
            });
    }
    else {
        handleDisconnect(error);
    }
}

void Session::handleWrite(const boost::system::error_code& error, size_t bytes) {
    if (error) {
        handleDisconnect(error);
    }
}

std::shared_ptr<Session> Session::create(boost::asio::io_context& io_context) {
	return std::shared_ptr<Session>(new Session(io_context));
}

boost::asio::ip::tcp::socket& Session::socket() {
	return client_socket_;
}

void Session::start() {
    boost::asio::async_read(client_socket_, boost::asio::buffer(&msg_length_, sizeof(msg_length_)), [self = shared_from_this()] (const boost::system::error_code& error, std::size_t bytes_transferred) {
            self->readHeader(error, bytes_transferred);
        });
}