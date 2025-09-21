#pragma once
#include <cstddef>
#include <string>
#include <cstdint>
#include <unordered_map>

#include <sys/epoll.h>

class TCPServer
{
public:
    TCPServer(int port) 
        : port_(port) 
    { }
    ~TCPServer();
    bool makeSocketNonBlocking(int fd);
    bool addToEpoll(int fd, uint32_t events = EPOLLIN | EPOLLET);
    void run();
    void stop();

private:

private:
    static constexpr size_t BUFFER_SIZE{ 1024 };
    static constexpr size_t MAX_EVENTS{ 1024 };

    int port_{ -1 };
    int serverFd_{ -1 };
    int epollFd_{ -1 };
    bool running_{ false };
    
    std::unordered_map<int, std::string> clientBuffers;
};