//
// Created by usman on 2/23/22.
//

#ifndef CONCURRENCY_SIMPLEMUTEXANDLOCK_H
#define CONCURRENCY_SIMPLEMUTEXANDLOCK_H

#include <mutex>
class SimpleMutexAndLock {
public:
    void run();


private:

    void threadExecute();

    std::mutex sequenceMutex;
};


#endif //CONCURRENCY_SIMPLEMUTEXANDLOCK_H
