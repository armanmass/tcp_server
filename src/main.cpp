#include "TCPServer.hpp"
#include "EpollHandler.hpp"
#include <memory>

int main()
{
    int serverPort{8080};
    TCPServer server{serverPort, std::make_unique<EpollHandler>()};
    server.run();
    server.stop();
}