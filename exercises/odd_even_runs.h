//
// Created by usman on 2/23/22.
//

#ifndef CONCURRENCY_ODD_EVEN_RUNS_H
#define CONCURRENCY_ODD_EVEN_RUNS_H

#include <thread>
#include <mutex>
#include <condition_variable>

class odd_even_runs {
public:
    void runOddEvenThreads(int threshold);
private:

    void startPrinting(int & start, int end, std::mutex &, std::condition_variable &, std::condition_variable &, bool );
    std::thread evenThread;
    std::thread oddThread;
};


#endif //CONCURRENCY_ODD_EVEN_RUNS_H
