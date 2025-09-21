#pragma once
#include <vector>
#include <thread>

class ThreadPool
{

private:
    static constexpr size_t numThreads { 4 };
    std::vector<std::thread> workers;
};