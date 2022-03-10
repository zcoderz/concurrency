//
// Created by usman on 3/10/22.
//

#ifndef CONCURRENCY_PARALLEL_FOR_EACH_TWO_H
#define CONCURRENCY_PARALLEL_FOR_EACH_TWO_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <execution>
#include <complex>
#include "timer.h"
#include "parallel_for_each.h"

class ParallelForEachExample {
    struct Data {
        double value;  // initial value
        double sqrt;   // parallel computed square root
    };
public:
    static void testMe() {
        int numElems = 10000000;

        // initialize numElems values without square root:
        std::vector<Data> coll;
        coll.reserve(numElems);
        for (int i = 0; i < numElems; ++i) {
            coll.push_back(Data{i * 4.37, 0});
        }

        // loop to make measurements mature:
        for (int i{0}; i < 5; ++i) {
            Timer t;
            // sequential execution:
            for_each(std::execution::seq,
                     coll.begin(), coll.end(),
                     [](auto &val) {
                         val.sqrt = std::sqrt(val.value);
                     });
            t.printDiff("sequential: ");

            // parallel execution:
            for_each(std::execution::par_unseq   ,
                     coll.begin(), coll.end(),
                     [](auto &val) {
                         val.sqrt = std::sqrt(val.value);
                     });
            t.printDiff("parallel:   ");

            ParallelForEach::parallel_for_each(coll.begin(), coll.end(),
                                               [](auto &val) {
                                                   val.sqrt = std::sqrt(val.value);
                                               });
            t.printDiff("parallel-manual:   ");

            std::cout << '\n';
        }
    }
};
#endif //CONCURRENCY_PARALLEL_FOR_EACH_TWO_H
