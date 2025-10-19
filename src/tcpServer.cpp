#include "../include/tcpServer.h"
#include "../include/session.h"

#include <iostream>
#include <filesystem>

TcpServer::TcpServer(boost::asio::io_context& io_context, int port) : io_context_(io_context), acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
    _Logger = Logger::get_instance();
    _Config = Config::get_instance();

    _Session = std::make_shared<Session>(io_context_);

    _Logger->addServerLog(_Logger->info, "(tcpServer) Server started on port: " + std::to_string(port), 2);

    start_accept(); 
}

void TcpServer::start_accept(){
    acceptor_.async_accept(_Session->socket(), std::bind(&TcpServer::handle_accept, this, boost::asio::placeholders::error));
}

void TcpServer::handle_accept(const boost::system::error_code& error){

    if (!error) {
        _Logger->addServerLog(_Logger->info, "", 2);
        _Logger->addServerLog(_Logger->info, "----------NEW----------", 2);
        _Logger->addServerLog(_Logger->info, "(tcpServer) Client connected. From ip: " + _Session->socket().remote_endpoint().address().to_string(), 2);

        _Session->start();
    }

    start_accept();
}