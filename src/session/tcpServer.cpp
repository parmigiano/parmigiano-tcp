#include "session/tcpServer.h"
#include "session/session.h"

#include <iostream>
#include <filesystem>

TcpServer::TcpServer(boost::asio::io_context& io_context, int port) : io_context_(io_context), acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
    _Logger = Logger::get_instance();
    _Config = Config::get_instance();

    //Session_ = std::make_shared<Session>(io_context_);

    _Logger->addServerLog(_Logger->info, MODULE_NAME_ + " Server started on port: " + std::to_string(port), 2);

    start_accept(); 
}

void TcpServer::start_accept(){
    auto new_session = std::make_shared<Session>(io_context_);

    acceptor_.async_accept(new_session->socket(),
        [this, new_session](const boost::system::error_code& error) {
            handle_accept(new_session, error);
    });

    //acceptor_.async_accept(Session_->socket(), std::bind(&TcpServer::handle_accept, this, boost::asio::placeholders::error));
}

void TcpServer::handle_accept(std::shared_ptr<Session> session, const boost::system::error_code& error){
    if (!error) {
        _Logger->addServerLog(_Logger->info, "", 2);
        _Logger->addServerLog(_Logger->info, "----------NEW----------", 2);
        _Logger->addServerLog(_Logger->info, MODULE_NAME_ + " Client connected. From ip: " + session->socket().remote_endpoint().address().to_string(), 2);

        session->start();
    }

    start_accept();
}