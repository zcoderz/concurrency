//
// Created by usman on 2/27/22.
//

#ifndef CONCURRENCY_PRODUCER_CONSUMER_WITH_FETCH_SUB_H
#define CONCURRENCY_PRODUCER_CONSUMER_WITH_FETCH_SUB_H
#include <atomic>
#include <thread>
#include <vector>
class ProducerConsumer{
    std::vector<int> queue_data;
    std::atomic<int> count;

    void wait_for_more_items() {}
    void process(int data){}

    void populate_queue()
    {
        unsigned const number_of_items=20;
        queue_data.clear();
        for(unsigned i=0;i<number_of_items;++i)
        {
            queue_data.push_back(i);
        }

        count.store(number_of_items,std::memory_order_release);
    }

    void consume_queue_items()
    {
        while(true)
        {
            int item_index;
            if((item_index=count.fetch_sub(1,std::memory_order_acquire))<=0)
            {
                wait_for_more_items();
                continue;
            }
            process(queue_data[item_index-1]);
        }
    }

    int testMain()
    {
        std::thread a(&ProducerConsumer::populate_queue, this);
        std::thread b(&ProducerConsumer::consume_queue_items, this);
        std::thread c(&ProducerConsumer::consume_queue_items, this);
        a.join();
        b.join();
        c.join();
    }
};
#endif //CONCURRENCY_PRODUCER_CONSUMER_WITH_FETCH_SUB_H
