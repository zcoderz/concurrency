//
// Created by usman on 2/28/22.
//

#ifndef CONCURRENCY_PARALLEL_PARTIAL_SUM_H
#define CONCURRENCY_PARALLEL_PARTIAL_SUM_H
#include <future>
#include <algorithm>
#include <vector>
#include <numeric>
#include "../utils/thread_joinable.h"
template<typename Iterator>
/**
 * this is a clever approach towards parallelling partial_sum which inherently appears to be a sequential algorithm
 * @tparam Iterator
 */
class parallelPartialSum {
    void parallel_partial_sum(Iterator first,Iterator last)
    {
        typedef typename Iterator::value_type value_type;
        struct process_chunk
        {
            void operator()(Iterator begin,Iterator last,
                            std::future<value_type>* previous_end_value,
                            std::promise<value_type>* end_value)
            {
                try
                {
                    Iterator end=last;
                    ++end;
                    //run the partial sum on the range begin to end
                    std::partial_sum(begin,end,begin);
                    if(previous_end_value) //previous_end_value is null for the first chunk
                    {
                        value_type& addend=previous_end_value->get(); //wait until previous end value is computed
                        *last+=addend;//add previous end value to the last element
                        //the last chunk doesnt have an end value. hence the check
                        if(end_value)
                        {
                            //trigger the promise so that dependent thread can proceed
                            end_value->set_value(*last);
                        }
                        std::for_each(begin,last,[addend](value_type& item)
                        {
                            //update the other items in range to add the prior last value
                            item+=addend;
                        });
                    }
                    else if(end_value)
                    {
                        //if this is the first chunk then just trigger the promise end_value
                        end_value->set_value(*last);
                    }
                }
                catch(...)
                {
                    //catch exception
                    if(end_value)
                    {
                        //end_value will exist for all chunks except for the last chunk
                        //set the exception on the promise. this exception will propagate forward to other threads
                        end_value->set_exception(std::current_exception());
                    }
                    else
                    {
                        //throw an exception if this is the last chunk, as this exception will propagate
                        //on the main thread
                        throw;
                    }
                }
            }
        };

        unsigned long const length=std::distance(first,last);

        if(!length)
            return last;

        //this can be much larger per thread (i.e. fill the cache line : 500-1000)
        unsigned long const min_per_thread=25;
        unsigned long const max_threads=
                (length+min_per_thread-1)/min_per_thread;

        unsigned long const hardware_threads=
                std::thread::hardware_concurrency();

        //carefully set the number of threads via utilizing number of processors and size of the chunks
        unsigned long const num_threads=
                std::min(hardware_threads!=0?hardware_threads:2,max_threads);

        unsigned long const block_size=length/num_threads;

        typedef typename Iterator::value_type value_type;

        std::vector<std::thread> threads(num_threads-1);
        std::vector<std::promise<value_type> > end_values(num_threads-1);
        std::vector<std::future<value_type> > previous_end_values;
        previous_end_values.reserve(num_threads-1); //pre allocate memory so that a given thread doesnt need to wait for memory allocations
        join_threads joiner(threads);

        Iterator block_start=first;
        //parallelize N-1 chunks
        for(unsigned long i=0;i<(num_threads-1);++i)
        {
            Iterator block_last=block_start;
            std::advance(block_last,block_size-1);
            threads[i]=std::thread(process_chunk(),
                                   block_start,block_last,
                                   (i!=0)?&previous_end_values[i-1]:0,
                                   &end_values[i]);
            block_start=block_last;
            ++block_start;
            previous_end_values.push_back(end_values[i].get_future());
        }
        Iterator final_element=block_start;
        std::advance(final_element,std::distance(block_start,last)-1);
        //run the last chunk in current thread
        process_chunk()(block_start,final_element,
                        (num_threads>1)?&previous_end_values.back():0,
                        0);
    }
};
#endif //CONCURRENCY_PARALLEL_PARTIAL_SUM_H
