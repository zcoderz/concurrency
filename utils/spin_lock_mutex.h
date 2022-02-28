//
// Created by usman on 2/27/22.
//

#ifndef CONCURRENCY_SPIN_LOCK_MUTEX_H
#define CONCURRENCY_SPIN_LOCK_MUTEX_H
#include <atomic>
class spin_lock_mutex
{
    std::atomic_flag flag;
public:
    spin_lock_mutex():
            flag(ATOMIC_FLAG_INIT)
    {}
    void lock()
    {
        while(flag.test_and_set(std::memory_order_acquire));
    }
    void unlock()
    {
        flag.clear(std::memory_order_release);
    }
};
#endif //CONCURRENCY_SPIN_LOCK_MUTEX_H
