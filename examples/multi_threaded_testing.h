//
// Created by usman on 2/28/22.
//

#ifndef CONCURRENCY_MULTI_THREADED_TESTING_H
#define CONCURRENCY_MULTI_THREADED_TESTING_H
#include <thread>
#include <future>
#include <cassert>
#include "../containers/thread_safe_queue.h"

/**
 * Example setup of multi threaded testing
 */
class MultiThreadedTesting{
    void test_concurrent_push_and_pop_on_empty_queue()
    {
        thread_safe_queue<int> q;

        std::promise<void> go,push_ready,pop_ready;
        std::shared_future<void> ready(go.get_future());

        std::future<void> push_done;
        std::future<std::shared_ptr<int>> pop_done;

        try
        {
            push_done=std::async(std::launch::async,
                                 [&q,ready,&push_ready]()
                                 {
                                     push_ready.set_value();
                                     ready.wait();
                                     q.push(42);
                                 }
            );
            pop_done=std::async(std::launch::async,
                                [&q,ready,&pop_ready]()
                                {
                                    pop_ready.set_value();
                                    ready.wait();
                                    return q.try_pop();
                                }
            );
            push_ready.get_future().wait();
            pop_ready.get_future().wait();
            go.set_value();

            push_done.get();
            assert(*pop_done.get()==42);
            assert(q.empty());
        }
        catch(...)
        {
            go.set_value();
            throw;
        }
    }

};
#endif //CONCURRENCY_MULTI_THREADED_TESTING_H
