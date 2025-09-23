#include "EpollHandler.hpp"

#include <atomic>
#include <cerrno>
#include <cstdio>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <stdexcept>
#include <print>

EpollHandler::EpollHandler()
    : events_(MAX_EVENTS)
{
    epoll_fd_ = epoll_create1(0);
    if (epoll_fd_ == -1)
    {
        perror("epoll_create1 failed");
        throw std::runtime_error("Failed to create epoll fd.\n");
    }

    event_fd_ = eventfd(0, EFD_NONBLOCK);
    if (event_fd_ == -1)
    {
        perror("eventfd failed");
        throw std::runtime_error("Failed to create event fd.\n");
    }

    epoll_event event{
        EPOLLIN,
        {.fd = event_fd_}
    };

    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, event_fd_, &event) == -1)
    {
        perror("epoll_ctl failed when adding event_fd_");
        throw std::runtime_error("Failed to add event fd to epoll instance.\n");
    }
}

EpollHandler::~EpollHandler()
{
    if (epoll_fd_ >= 0) close(epoll_fd_);
    if (event_fd_ >= 0) close(event_fd_);
}

bool EpollHandler::addConnection(int client_fd)
{
    if (client_fd < 0) return false;

    epoll_event event{
        EPOLLIN | EPOLLET,
        {.fd = client_fd}
    };

    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, client_fd, &event) == -1)
    {
        perror("epoll_ctl failed for client_fd in EpollHanlder::addConnection");
        return false;
    }

    return true;
}

void EpollHandler::run()
{
    running_.store(true, std::memory_order_release);
    std::byte buffer[READ_BUFFER_SIZE]{};

    while (running_)
    {
        int numEvents = epoll_wait(epoll_fd_, events_.data(), MAX_EVENTS, -1);
        if (numEvents == -1)
        {
            if (errno == EINTR) continue;
            perror("epoll_wait failed");
            break;
        }

        for (size_t i{}; i < numEvents; ++i)
        {
            auto currFd = events_[i].data.fd;
            auto currEvents = events_[i].events;

            if (currFd == event_fd_)
            {
                running_.store(false, std::memory_order_release);
                break;
            }

            if (currEvents & (EPOLLERR | EPOLLHUP))
            {
                std::println(stderr, "Epoll error one fd: {}", currFd);
                close(currFd);
                continue;
            }

            if (currEvents & EPOLLIN)
            {
                while (true)
                {
                    ssize_t count = read(currFd, &buffer, READ_BUFFER_SIZE);

                    if (count == -1)
                    {
                        if (errno != EAGAIN)
                        {
                            perror("read failed");
                            close(currFd);
                        }
                        break;
                    }
                    else if (count == 0)
                    {
                        close(currFd);
                        break;
                    }
                    else
                    {
                        write(currFd, &buffer, count);
                    }
                }
            }
        }
    }
}

void EpollHandler::stop()
{
    if (running_.exchange(false, std::memory_order_acq_rel))
    {
        size_t exit{};
        if (write(event_fd_, &exit, sizeof(exit)) == -1)
        {
            perror("write to event_fd_ failed");
        }
    }
}