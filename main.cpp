
#include <iostream>
#include "exercises/OddEvenRuns.h"
#include "exercises/SimpleMutexAndLock.h"



int main() {
    const auto workers = {"anil", "busara", "carl"};

    auto on_completion = []() noexcept {
        // locking not needed here
        static auto phase = "... done\n" "Cleaning up...\n";
         std::cout << phase;
        phase = "... done22\n";
    };
    std::barrier sync_point(std::ssize(workers), on_completion);

    auto work = [&](std::string name) {
        std::string product = "  " + name + " worked\n";
        std::cout << product;  // ok, op<< call is atomic
        sync_point.arrive_and_wait();

        product = "  " + name + " cleaned\n";
        std::cout << product;
        sync_point.arrive_and_wait();
    };

    std::cout << "Starting...\n";
    std::vector<std::thread> threads;
    for (auto const &worker: workers) {
        threads.emplace_back(work, worker);
        sleep(1);
    }
    for (auto &thread: threads) {
        thread.join();
    }
}
//int main() {
//    SimpleMutexAndLock simpleMutexAndLock;
//    simpleMutexAndLock.run();
//    //OddEvenRuns oddEvenRuns;
//    //oddEvenRuns.runOddEvenThreads(50);
//    return 0;
//}
