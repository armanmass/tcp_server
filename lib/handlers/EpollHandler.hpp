#pragma once
#include "IConnectionHandler.hpp"

#include <sys/epoll.h>
#include <atomic>
#include <vector>

class EpollHandler : public IConnectionHandler
{
public:
    EpollHandler();
    ~EpollHandler() override;
    bool addConnection(int client_fd) override;
    void run() override;
    void stop() override;
private:
    static constexpr int MAX_EVENTS{ 10 };
    static constexpr int READ_BUFFER_SIZE{ 1024 };

    int epoll_fd_{ -1 };
    int event_fd_{ -1 };

    std::atomic<bool> running_{ false };
    std::vector<struct epoll_event> events_;
};