//
// Created by usman on 2/25/22.
//

#ifndef CONCURRENCY_PROMISE_EXAMPLE_H
#define CONCURRENCY_PROMISE_EXAMPLE_H
#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <iostream>
#include <chrono>
class PromiseExample {


    void accumulate(std::vector<int>::iterator first,
                    std::vector<int>::iterator last,
                    std::promise<int> accumulate_promise)
    {
        int sum = std::accumulate(first, last, 0);
        accumulate_promise.set_value(sum);  // Notify future
    }

    void do_work(std::promise<void>   barrier)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        barrier.set_value();
    }

    int testPromise()
    {
        // Demonstrate using promise<int> to transmit a result between threads.
        std::vector<int> numbers = { 1, 2, 3, 4, 5, 6 };
        std::promise<int> accumulate_promise;
        std::future<int> accumulate_future = accumulate_promise.get_future();
        std::thread work_thread(&PromiseExample::accumulate, this, numbers.begin(), numbers.end(),
                                std::move(accumulate_promise));

        // future::get() will wait until the future has a valid result and retrieves it.
        // Calling wait() before get() is not needed
        //accumulate_future.wait();  // wait for result
        std::cout << "result=" << accumulate_future.get() << '\n';
        work_thread.join();  // wait for thread completion

        // Demonstrate using promise<void> to signal state between threads.
        std::promise<void> barrier;
        std::future<void> barrier_future = barrier.get_future();
        std::thread new_work_thread(&PromiseExample::do_work, this, std::move(barrier));
        barrier_future.wait();
        new_work_thread.join();
    }
};
#endif //CONCURRENCY_PROMISE_EXAMPLE_H
