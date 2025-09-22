#pragma once
#include <vector>
#include <thread>

class ThreadPool
{
public:

private:
    static constexpr size_t numThreads { 4 };
    std::vector<std::thread> workers;
};