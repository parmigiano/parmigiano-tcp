#include "../include/session.h"

#include "../include/usersQueue.h"

#include <iostream>

Session::Session(boost::asio::io_context& io_context) : socket_(io_context) {
    _Logger = Logger::get_instance();
    _Config = Config::get_instance();

    _Config->write_handler_ptr = [this] (const boost::system::error_code& error, size_t bytes) {
        this->handle_write(error, bytes); 
    };

    _UsersQueue = std::make_shared<UsersQueue>();
}

void Session::handle_disconnect(const boost::system::error_code& error) {
    if (error == boost::asio::error::eof) {
        _Logger->addServerLog(_Logger->info, "(session) Client disconnected gracefully", 2);
    }
    else if (error == boost::asio::error::connection_reset) {
        _Logger->addServerLog(_Logger->info, "(session) Client connection reset", 2);
    }
    else {
        _Logger->addServerLog(_Logger->info, "(session) Client connection error: " + error.message(), 2);
    }

    boost::system::error_code ignore_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignore_ec);
    socket_.close(ignore_ec);
}

void Session::handle_read(const boost::system::error_code& error, size_t bytes) {
    if (!error) {
        std::string data(data_, bytes);

        _UsersQueue->addUserToQueue(data, Session::socket());   

        start();
    }
    else {
        handle_disconnect(error);
    }
}

void Session::handle_write(const boost::system::error_code& error, size_t bytes) {
    if (error) {
        handle_disconnect(error);
    }
}

Session::pointer Session::create(boost::asio::io_context& io_context) {
	return pointer(new Session(io_context));
}

boost::asio::ip::tcp::socket& Session::socket() {
	return socket_;
}

void Session::start() {
	socket_.async_read_some(boost::asio::buffer(data_, max_length), [this](const boost::system::error_code& error, std::size_t bytes_transferred) {handle_read(error, bytes_transferred); });
}