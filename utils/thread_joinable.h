//
// Created by usman on 2/27/22.
//

#ifndef CONCURRENCY_THREAD_JOINABLE_H
#define CONCURRENCY_THREAD_JOINABLE_H
#include <vector>
#include <thread>
struct join_threads
{
    std::vector<std::thread> &_threads;
    join_threads(std::vector<std::thread> &threads_) : _threads(threads_)
    {}
    ~join_threads() {
        for (std::thread & thread: _threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
};
#endif //CONCURRENCY_THREAD_JOINABLE_H
