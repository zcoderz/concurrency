//
// Created by usman on 2/23/22.
//

#include "SimpleMutexAndLock.h"
#include <iostream>
#include <thread>

using namespace std;


void SimpleMutexAndLock::run() {
    thread t1 = thread(&SimpleMutexAndLock::threadExecute, this);
    thread t2 = thread(&SimpleMutexAndLock::threadExecute, this);
    thread t3 = thread(&SimpleMutexAndLock::threadExecute, this);
    thread t4 = thread(&SimpleMutexAndLock::threadExecute, this);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
void SimpleMutexAndLock::threadExecute() {
    for (int i =0; i < 1000;  i++) {
        {
            scoped_lock lock(sequenceMutex);
            cout << this_thread::get_id() << " -- " << i << endl;
        }
    }
}