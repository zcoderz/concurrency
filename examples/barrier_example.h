//
// Created by usman on 2/25/22.
//

#ifndef CONCURRENCY_BARRIER_EXAMPLE_H
#define CONCURRENCY_BARRIER_EXAMPLE_H
#include <barrier>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
class BarrierExample {

    void testBarrier() {
        const auto workers = {"anil", "busara", "carl"};

        /**
         * this method is set to be called when the barrier reaches 0 (i.e arrive_and_wait has been called N times).
         * it will only be called once
         */
        auto on_completion = []() noexcept {
            // locking not needed here
            static auto phase = "... done\n" "Cleaning up...\n";
            std::cout << phase;
            phase = "... done22\n";
        };
        //set up the barrier
        std::barrier sync_point(std::ssize(workers), on_completion);

        auto work = [&](std::string name) {
            std::string product = "  " + name + " worked\n";
            std::cout << product;  // ok, op<< call is atomic
            //this call will block until all threads have called arrive and wait.
            //each decrements the count in the barrier. once it reaches 0 (i.e. all threads have called this method)
            //threads can proceed and the barrier is reset
            sync_point.arrive_and_wait();

            product = "  " + name + " cleaned\n";
            std::cout << product;
            //the barrier is reused here - same as above
            sync_point.arrive_and_wait();
        };

        std::cout << "Starting...\n";
        std::vector<std::thread> threads;
        for (auto const &worker: workers) {
            //starts the threads -- clean approach to creating threads via vector emplace_back
            threads.emplace_back(work, worker);
            sleep(1);
        }

        for (auto &thread: threads) {
            thread.join();
        }
    }
};

#endif //CONCURRENCY_BARRIER_EXAMPLE_H
