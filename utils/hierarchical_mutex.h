//
// Created by usman on 2/24/22.
//

#ifndef CONCURRENCY_HIERARCHICAL_MUTEX_H
#define CONCURRENCY_HIERARCHICAL_MUTEX_H

#endif //CONCURRENCY_HIERARCHICAL_MUTEX_H

#include <mutex>
#include <stdexcept>
#include <climits>

/**
 * Can construct a set of hierarchical mutexes and assign each mutex a number based on hierarchy values
 * per this design you take mutexes from higher to lower order. taking mutexes in opposite order will cause violation
 *
 */
class hierarchical_mutex
{
    std::mutex internal_mutex;
    unsigned long const hierarchy_value;
    unsigned long previous_hierarchy_value;
    //the variable is thread local so that it belongs per thread
    static thread_local unsigned long this_thread_hierarchy_value;

    void check_for_hierarchy_violation()
    {
        if(this_thread_hierarchy_value <= hierarchy_value)
        {
            throw std::logic_error("mutex hierarchy violated");
        }
    }
    void update_hierarchy_value()
    {
        previous_hierarchy_value=this_thread_hierarchy_value;
        this_thread_hierarchy_value=hierarchy_value;
    }
public:
    explicit hierarchical_mutex(unsigned long value):
            hierarchy_value(value),
            previous_hierarchy_value(0)
    {}
    void lock()
    {
        check_for_hierarchy_violation();
        internal_mutex.lock();
        update_hierarchy_value();
    }
    void unlock()
    {
        this_thread_hierarchy_value=previous_hierarchy_value;
        internal_mutex.unlock();
    }
    bool try_lock()
    {
        check_for_hierarchy_violation();
        if(!internal_mutex.try_lock())
            return false;
        update_hierarchy_value();
        return true;
    }
};
thread_local unsigned long
        hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);