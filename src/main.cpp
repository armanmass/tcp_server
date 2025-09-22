#include "TCPServer.hpp"
#include "EpollHandler.hpp"
#include <memory>

int main()
{
    TCPServer server{8080, std::make_unique<EpollHandler>()};
    server.run();
    server.stop();
}