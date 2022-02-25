//
// Created by usman on 2/23/22.
//

#ifndef CONCURRENCY_ODDEVENRUNS_H
#define CONCURRENCY_ODDEVENRUNS_H

#include <thread>
#include <mutex>
#include <condition_variable>

class OddEvenRuns {
public:
    void runOddEvenThreads(int threshold);
private:

    void startPrinting(int & start, int end, std::mutex &, std::condition_variable &, std::condition_variable &, bool );
    std::thread evenThread;
    std::thread oddThread;
};


#endif //CONCURRENCY_ODDEVENRUNS_H
