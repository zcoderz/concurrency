//
// Created by usman on 2/25/22.
//

#ifndef CONCURRENCY_LATCH_EXAMPLE_H
#define CONCURRENCY_LATCH_EXAMPLE_H
#include <functional>
#include <iostream>
#include <latch>
#include <string>
#include <thread>

class LatchExample {
    void latchExample() {
        struct job {
            const std::string name;
            std::string product{"not worked"};
            std::thread action{};
        } jobs[] = {{"annika"}, {"buru"}, {"chuck"}};

        std::latch work_done{std::size(jobs)};
        std::latch start_clean_up{1};

        auto work = [&](job& my_job) {
            my_job.product = my_job.name + " worked";
            //moves the latch down
            work_done.count_down();
            //waits until latch is ready
            start_clean_up.wait();
            my_job.product = my_job.name + " cleaned";
        };

        std::cout << "Work starting... ";
        for (auto& job : jobs) {
            job.action = std::thread{work, std::ref(job)};
        }
        //waits until all work is done.
        work_done.wait();
        std::cout << "done:\n";
        for (auto const& job : jobs) {
            std::cout << "  " << job.product << '\n';
        }

        std::cout << "Workers cleaning up... ";
        //identifies cleanup can start by reducing cleanup latch once all work has been completed
        start_clean_up.count_down();
        //wait for all threads to complete
        for (auto& job : jobs) {
            job.action.join();
        }
        std::cout << "done:\n";
        for (auto const& job : jobs) {
            std::cout << "  " << job.product << '\n';
        }
    }
};
#endif //CONCURRENCY_LATCH_EXAMPLE_H
