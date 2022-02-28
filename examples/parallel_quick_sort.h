//
// Created by usman on 2/27/22.
//

#ifndef CONCURRENCY_PARALLEL_QUICK_SORT_H
#define CONCURRENCY_PARALLEL_QUICK_SORT_H
#include <thread>
#include <vector>
#include <future>
#include <list>
#include <memory>
#include "../containers/thread_safe_stack.h"
template<typename T>
class ParallelQuickSort {
public:
    std::list<T> parallel_quick_sort(std::list<T> input)
    {
        if(input.empty())
        {
            return input;
        }
        ParallelQuickSort s;
        return s.do_sort(input);
    }
private:
    struct chunk_to_sort
    {
        std::list<T> data;
        std::promise<std::list<T> > promise;
    };
    threadsafe_stack<chunk_to_sort> chunks;
    std::vector<std::thread> threads;
    unsigned const max_thread_count;
    std::atomic<bool> end_of_data;

    ParallelQuickSort():
            max_thread_count(std::thread::hardware_concurrency()-1),
            end_of_data(false)
    {}

    ~ParallelQuickSort()
    {
        end_of_data=true;
        for(unsigned i=0;i<threads.size();++i)
        {
            threads[i].join();
        }
    }

    /**
     * Take chunk from stack if one exists and process it if possible
     */
    void try_sort_chunk()
    {
        std::shared_ptr<chunk_to_sort > chunk=chunks.pop();
        if(chunk)
        {
            sort_chunk(chunk);
        }
    }

    /**
     * The main executor to process sorting of a certain chunk. It divides the data into partitions based on
     * the position of the first element in the list.
     * This can lead to random allocation of chunks. A better approach is to use an external merge sort algorithm
     * for sorting large chunks.
     * However, this method is good to understand conceptually how parallelization can be implemented
     * @param chunk_data
     * @return
     */
    std::list<T> do_sort(std::list<T>& chunk_data)
    {
        if(chunk_data.empty())
        {
            return chunk_data;
        }

        std::list<T> result;
        result.splice(result.begin(),chunk_data,chunk_data.begin());
        T const& partition_val=*result.begin();

        //divide the list into segments based on partition
        typename std::list<T>::iterator divide_point=
                std::partition(chunk_data.begin(),chunk_data.end(),
                               [&](T const& val){return val<partition_val;});
        chunk_to_sort new_lower_chunk;
        //create a lower chunk of the data based on the divide point
        new_lower_chunk.data.splice(new_lower_chunk.data.end(),
                                    chunk_data,chunk_data.begin(),
                                    divide_point);
        //get a future based on the chunk
        std::future<std::list<T> > new_lower=
                new_lower_chunk.promise.get_future();
        chunks.push(std::move(new_lower_chunk));
        if(threads.size()<max_thread_count)
        {
            //create a new thread if number of threads is less than the target
            threads.push_back(std::thread(&ParallelQuickSort<T>::sort_thread,this));
        }

        //process the remaining chunk in current thread
        std::list<T> new_higher(do_sort(chunk_data));

        result.splice(result.end(),new_higher);
        while(new_lower.wait_for(std::chrono::seconds(0)) !=
              std::future_status::ready)
        {
            //while lower chunk is not processed , use the current thread to process other chunks
            try_sort_chunk();
        }
        //merge the lower chunk into the upper chunk
        result.splice(result.begin(),new_lower.get());
        return result;
    }

    void sort_chunk(std::shared_ptr<chunk_to_sort > const& chunk)
    {
        chunk->promise.set_value(do_sort(chunk->data));
    }

    void sort_thread()
    {
        while(!end_of_data)
        {
            try_sort_chunk();
            //stop execution of current thread and give priority to other threads
            std::this_thread::yield();
        }
    }


};
#endif //CONCURRENCY_PARALLEL_QUICK_SORT_H
