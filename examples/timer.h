//
// Created by usman on 3/10/22.
//

#ifndef CONCURRENCY_TIMER_H
#define CONCURRENCY_TIMER_H
#include <iostream>
#include <string>
#include <chrono>


/********************************************
 * timer to print elapsed time
 ********************************************/

class Timer
{
private:
    std::chrono::steady_clock::time_point last;
public:
    Timer()
            : last{std::chrono::steady_clock::now()} {
    }
    void printDiff(const std::string& msg = "Timer diff: ") {
        auto now{std::chrono::steady_clock::now()};
        std::chrono::duration<double, std::milli> diff{now - last};
        std::cout << msg << diff.count() << "ms\n";
        last = std::chrono::steady_clock::now();
    }
};

#endif //CONCURRENCY_TIMER_H
