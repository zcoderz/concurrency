//
// Created by usman on 3/10/22.
//

#ifndef CONCURRENCY_PARALLEL_TRANSFORM_REDUCE_H
#define CONCURRENCY_PARALLEL_TRANSFORM_REDUCE_H

#include <vector>
#include <execution>
#include <iostream>

class ParallelReduce {
    static void printSum(long num)
    {
        // create coll with num sequences of 1 2 3 4:
        std::vector<long> coll;
        coll.reserve(num * 4);
        for (long i=0; i < num; ++i) {
            coll.insert(coll.end(), {1, 2, 3, 4});
        }

        auto squaredSum = [] (auto val) {
            return val * val;
        };

        auto sum = std::transform_reduce(std::execution::par,
                               coll.begin(), coll.end(),
                               0L,
                               std::plus{}, //reduce op
                               squaredSum); //transform op
        std::cout << "transform reduce():     " << sum << '\n';
    }

public:
    static void testMe()
    {
        printSum(1);
        printSum(1000);
        printSum(1000000);
        printSum(10000000);
    }

};

#endif //CONCURRENCY_PARALLEL_TRANSFORM_REDUCE_H
