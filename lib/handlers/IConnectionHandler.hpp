#pragma once

class IConnectionHandler
{
public:
    virtual ~IConnectionHandler() = default;
    virtual bool addConnection(int client_fd) = 0;
    virtual void run() = 0;
    virtual void stop() = 0;
};