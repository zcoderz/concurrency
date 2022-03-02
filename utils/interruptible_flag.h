//
// Created by usman on 3/1/22.
//

#ifndef CONCURRENCY_INTERRUPTIBLE_FLAG_H
#define CONCURRENCY_INTERRUPTIBLE_FLAG_H

#include <future>
#include <algorithm>

class interrupt_flag
{
    std::atomic<bool> flag;
    std::condition_variable* thread_cond;
    std::mutex set_clear_mutex;

public:
    interrupt_flag():
            thread_cond(0)
    {}

    void set()
    {
        flag.store(true,std::memory_order_relaxed);
        std::lock_guard<std::mutex> lk(set_clear_mutex);
        if(thread_cond)
        {
            thread_cond->notify_all();
        }
    }
    bool is_set() const
    {
        return flag.load(std::memory_order_relaxed);
    }

    void set_condition_variable(std::condition_variable& cv)
    {
        std::lock_guard<std::mutex> lk(set_clear_mutex);
        thread_cond=&cv;
    }

    void clear_condition_variable()
    {
        std::lock_guard<std::mutex> lk(set_clear_mutex);
        thread_cond=0;
    }

    struct clear_cv_on_destruct
    {
        interrupt_flag & _flag;

        clear_cv_on_destruct(interrupt_flag & flag_) : _flag(flag_) {}

        ~clear_cv_on_destruct()
        {
            _flag.clear_condition_variable();
        }
    };
};


//example code of how to use an
class TestInterruptPoint {
    static interrupt_flag this_thread_interrupt_flag;

    void interruption_point() {
        if (this_thread_interrupt_flag.is_set()) {
            //throw exception to signal that the thread has been interrupted
        }
    };

    void interruptible_wait(std::condition_variable& cv,
                            std::unique_lock<std::mutex>& lk) {
        interruption_point();
        interrupt_flag this_thread_interrupt_flag;
        this_thread_interrupt_flag.set_condition_variable(cv);
        interrupt_flag::clear_cv_on_destruct guard(this_thread_interrupt_flag);
        interruption_point();
        cv.wait_for(lk,std::chrono::milliseconds(1));
        interruption_point();
    }
};

#endif //CONCURRENCY_INTERRUPTIBLE_FLAG_H
