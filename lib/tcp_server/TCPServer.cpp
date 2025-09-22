#include "TCPServer.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <cstdio>
#include <thread>
#include <stdexcept>

TCPServer::TCPServer(int port, std::unique_ptr<IConnectionHandler> handler)
    : port_(port)
    , handler_(std::move(handler))
{
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ == -1)
    {
        perror("socket failed");
        throw std::runtime_error("Failed to create TCP Server socket.\n");
    }

    int opt{ 1 };
    if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        perror("setsockopt failed");
        throw std::runtime_error("Failed to set TCP Server socket options.\n");
    }

    sockaddr_in address{
        AF_INET,
        htons(port_),
        INADDR_ANY 
    };

    if (bind(server_fd_, reinterpret_cast<const struct sockaddr*>(&address), sizeof(address)) == -1)
    {
        perror("bind failed");
        throw std::runtime_error("Failed to bind socket.\n");
    }

    if (listen(server_fd_, BACKLOG_QUEUE_LENGTH) == -1)
    {
        perror("listen failed:");
        throw std::runtime_error("Failed to listen.\n");
    }
}


TCPServer::~TCPServer()
{
    if (server_fd_ >= 0)
        close(server_fd_);
}


void TCPServer::run()
{
    std::jthread handlerWorker{&IConnectionHandler::run, handler_.get()};
    running_ = true;

    while (running_)
    {   
        sockaddr_in clientAddress{};
        socklen_t socketLen{sizeof(clientAddress)};
        int clientFd = accept(server_fd_, reinterpret_cast<struct sockaddr*>(&clientAddress), &socketLen);
        if (clientFd == -1)
        {
            if (running_) { perror("accept failed"); }
            continue;
        }
        handler_->addConnection(clientFd);
    }
}


void TCPServer::stop() 
{ 
    running_ = false; 
    if (server_fd_ >= 0)
    {
        shutdown(server_fd_, SHUT_RDWR);
        close(server_fd_);
        server_fd_ = -1;
    }
    handler_->stop();
}