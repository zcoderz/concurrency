//
// Created by usman on 2/23/22.
//

#include "OddEvenRuns.h"
#include <iostream>


using namespace std;

void OddEvenRuns::runOddEvenThreads(int threshold) {
    mutex sequenceMutex;
    condition_variable cvOdd;
    condition_variable cvEven;
    int origValue=0;
    evenThread = std::thread(&OddEvenRuns::startPrinting, this, std::ref(origValue), threshold, std::ref(sequenceMutex), ref(cvEven), ref(cvOdd), true);
    oddThread = std::thread(&OddEvenRuns::startPrinting, this, std::ref(origValue), threshold, std::ref(sequenceMutex), ref(cvOdd), ref(cvEven),  false);
    evenThread.join();
    oddThread.join();
}

void OddEvenRuns::startPrinting(int & index, int end, mutex & sequenceMutex, condition_variable & cvA, condition_variable & cvB, bool isEven) {

    while (index <= end) {
        unique_lock guard(sequenceMutex);
        if (isEven) {
            cvA.wait (guard, [&index] { return index % 2 == 0; });
        } else {
            cvA.wait (guard, [&index] { return index % 2 == 1; });
        }
        if (index < end) {
            cout << this_thread::get_id() << "--" << index << endl;
        }
        cvB.notify_one();
        index++;
    }
}