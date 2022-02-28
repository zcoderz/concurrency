//
// Created by usman on 2/27/22.
//

#ifndef CONCURRENCY_RELEASE_ACQUIRE_EXAMPLE_H
#define CONCURRENCY_RELEASE_ACQUIRE_EXAMPLE_H

#include <atomic>
#include <thread>
#include <assert.h>

class ReleaseAcquire {


    std::atomic<bool> x, y;
    std::atomic<int> z;

    void write_x() {
        x.store(true, std::memory_order_release);
    }

    void write_y() {
        y.store(true, std::memory_order_release);
    }

    void read_x_then_y() {
        while (!x.load(std::memory_order_acquire));
        if (y.load(std::memory_order_acquire))
            ++z;
    }

    void read_y_then_x() {
        while (!y.load(std::memory_order_acquire));
        if (x.load(std::memory_order_acquire))
            ++z;
    }

    int testMain() {
        x = false;
        y = false;
        z = 0;
        std::thread a(&ReleaseAcquire::write_x, this);
        std::thread b(&ReleaseAcquire::write_y, this);
        std::thread c(&ReleaseAcquire::read_x_then_y, this);
        std::thread d(&ReleaseAcquire::read_y_then_x, this);
        a.join();
        b.join();
        c.join();
        d.join();
        assert(z.load() != 0);
    }

};

#endif //CONCURRENCY_RELEASE_ACQUIRE_EXAMPLE_H
