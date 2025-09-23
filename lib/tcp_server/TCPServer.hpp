#pragma once

#include "IConnectionHandler.hpp"
#include <cstddef>
#include <memory>

class TCPServer
{
public:
    TCPServer(int port, std::unique_ptr<IConnectionHandler> handler);
    ~TCPServer();
    void run();
    void stop();

private:
    std::unique_ptr<IConnectionHandler> handler_;
    int server_fd_{ -1 }; 
    int port_{ -1 };
    bool running_{ false };
    static constexpr size_t BACKLOG_QUEUE_LENGTH{ 10 };
};
