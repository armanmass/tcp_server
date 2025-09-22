#pragma once
#include "IConnectionHandler.hpp"

class EpollHandler : public IConnectionHandler
{
public:
    ~EpollHandler() = default;
    bool addConnection(int client_fd){ return false; }
    void run(){}
    void stop(){}
};