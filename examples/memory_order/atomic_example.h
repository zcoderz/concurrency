//
// Created by usman on 2/27/22.
//

#ifndef CONCURRENCY_ATOMIC_EXAMPLE_H
#define CONCURRENCY_ATOMIC_EXAMPLE_H

#include <vector>
#include <atomic>
#include <iostream>
#include <chrono>
#include <thread>

class AtomicExample {
    std::vector<int> data;
    std::atomic_bool data_ready = false;

    void reader_thread() {
        while (!data_ready.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        std::cout << "The answer=" << data[0] << "\n";
    }

    void writer_thread() {
        data.push_back(42);
        data_ready = true;
    }

};

#endif //CONCURRENCY_ATOMIC_EXAMPLE_H
