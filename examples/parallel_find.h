//
// Created by usman on 2/27/22.
//

#ifndef CONCURRENCY_PARALLEL_FIND_H
#define CONCURRENCY_PARALLEL_FIND_H
#include <atomic>
#include <future>
template<typename Iterator,typename MatchType>
class ParallelFind {
public:
    Iterator parallel_find(Iterator first,Iterator last,MatchType match)
    {
        std::atomic<bool> done(false);
        return parallel_find_impl(first,last,match,done);
    }
private:
    Iterator parallel_find_impl(Iterator first,Iterator last,MatchType match,
                                std::atomic<bool>& done)
    {
        try
        {
            unsigned long const length=std::distance(first,last);
            unsigned long const min_per_thread=25;
            if(length<(2*min_per_thread))
            {
                for(;(first!=last) && !done.load();++first)
                {
                    if(*first==match)
                    {
                        done=true;
                        return first;
                    }
                }
                return last;
            }
            else
            {
                Iterator const mid_point=first+(length/2);
                std::future<Iterator> async_result=
                        std::async(&ParallelFind::parallel_find_impl, this,
                                   mid_point,last,match,std::ref(done));
                Iterator const direct_result=
                        parallel_find_impl(first,mid_point,match,done);
                return (direct_result==mid_point)?
                       async_result.get():direct_result;
            }
        }
        catch(...)
        {
            done=true;
            throw;
        }
    }
};
#endif //CONCURRENCY_PARALLEL_FIND_H
