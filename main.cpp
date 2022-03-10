
#include <iostream>
#include "exercises/odd_even_runs.h"
#include "exercises/simple_mutex_and_lock.h"
#include "examples/parallel_page_visits.h"
#include "examples/background_interruptible_threads.h"
#include "examples/parallel_for_each_two.h"
#include "examples/parallel_transform_reduce.h"

#include <functional>
#include <iostream>
#include <latch>
#include <string>
#include <thread>
#include <cassert>


int main() {
    unsigned long const hardware_threads=
            std::thread::hardware_concurrency();

    ParallelReduce::testMe();
    //ParallelForEachExample::testMe();
    //TestInterruptableThreadsExample interruptableThreadsExample;
    //interruptableThreadsExample.run();
//    ParallelPageVisits parallelPageVisits;
//    std::vector<std::string> lines = parallelPageVisits.create_log_lines();
//    visit_map_type vMap = parallelPageVisits.count_visits_per_page(lines);
//    for (const auto &[k, v] : vMap)
//        std::cout << "m[" << k << "] = (" << v << ") " << std::endl;

    //simple_mutex_and_lock simpleMutexAndLock;
    //simpleMutexAndLock.run();
    //odd_even_runs oddEvenRuns;
    //oddEvenRuns.runOddEvenThreads(50);
    return 0;
}
