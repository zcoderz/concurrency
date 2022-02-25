//
// Created by usman on 2/24/22.
//

#ifndef CONCURRENCY_WAIT_UNTIL_H
#define CONCURRENCY_WAIT_UNTIL_H
#include <condition_variable>
#include <mutex>
#include <chrono>

class WaitUntil {

    bool waitUntilExample() {
        std::condition_variable cv;
        bool done;
        std::mutex m;
        auto const timeout = std::chrono::steady_clock::now() +
                             std::chrono::milliseconds(500);
        std::unique_lock<std::mutex> lk(m);
        while (!done) {
            if (cv.wait_until(lk, timeout) == std::cv_status::timeout)
                break;
        }
        return done;
    }
};
#endif //CONCURRENCY_WAIT_UNTIL_H
