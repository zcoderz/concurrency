//
// Created by usman on 2/23/22.
//

#include "simple_mutex_and_lock.h"
#include <iostream>
#include <thread>

using namespace std;


void simple_mutex_and_lock::run() {
    thread t1 = thread(&simple_mutex_and_lock::threadExecute, this);
    thread t2 = thread(&simple_mutex_and_lock::threadExecute, this);
    thread t3 = thread(&simple_mutex_and_lock::threadExecute, this);
    thread t4 = thread(&simple_mutex_and_lock::threadExecute, this);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
void simple_mutex_and_lock::threadExecute() {
    for (int i =0; i < 1000;  i++) {
        {
            scoped_lock lock(sequenceMutex);
            cout << this_thread::get_id() << " -- " << i << endl;
        }
    }
}