//
// Created by usman on 2/23/22.
//

#ifndef CONCURRENCY_SIMPLE_MUTEX_AND_LOCK_H
#define CONCURRENCY_SIMPLE_MUTEX_AND_LOCK_H

#include <mutex>
class simple_mutex_and_lock {
public:
    void run();


private:

    void threadExecute();

    std::mutex sequenceMutex;
};


#endif //CONCURRENCY_SIMPLE_MUTEX_AND_LOCK_H
