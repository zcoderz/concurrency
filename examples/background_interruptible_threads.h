//
// Created by usman on 3/1/22.
//

#ifndef CONCURRENCY_BACKGROUND_INTERRUPTIBLE_THREADS_H
#define CONCURRENCY_BACKGROUND_INTERRUPTIBLE_THREADS_H

#include <thread>
#include <iostream>
#include <ctime>
#include <chrono>
#include "../utils/interruptible_flag.h"

thread_local interrupt_flag this_thread_interrupt_flag;


std::string getCurrentTime() {
    auto time = std::time(nullptr);
    std::stringstream str;
    str <<   std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return str.str();
}

/**
 * Class wraps c++ thread with an interrupt flag that is used to signal thread to be interrupted/stopped
 */
class InterruptibleThread {
    std::thread internal_thread;
    interrupt_flag *flag;
public:
    template<typename FunctionType, typename Arg>
    InterruptibleThread(FunctionType f, Arg arg) {
        //create a promise and set interruption flag in it as part of the execution lambda for the thread
        //this will ensure that flag is set only when the thread has been started and hence provide a strong
        //consistency between the state of flag and the start of thread
        std::promise<interrupt_flag *> p;
        internal_thread = std::thread([f, &p, arg] {
            p.set_value(&this_thread_interrupt_flag);
            f(arg);
        });
        flag = p.get_future().get();
    }

    /**
     * Makes a call to interrupt the thread
     */
    void interrupt() {
        if (flag) {
            flag->set();
        }
    }

    /**
     * Join the thread
     */
    void join() {
        internal_thread.join();
    }
};

struct interruptException: std::exception
{
    const char* what() const throw()
    {
        return "thread interrupted";
    }

};

class TestInterruptableThreadsExample {
private:
    //std::mutex config_mutex;
    std::vector<InterruptibleThread> background_threads;

    /**
     * checks the thread local variable interrupt to see if interruption flag has been set
     * @return
     */
    static bool interruption_point() {
        if (this_thread_interrupt_flag.is_set()) {
            return true;
            //throw interruptException();
        }
        return false;
    }

    /**
     * background thread does background work
     * @param disk_id
     */
    static void background_thread(int disk_id) {
        while (true) {
            bool  doStop = interruption_point();
            if (doStop) {
                return;
            }
            //fs_change fsc=get_fs_changes(disk_id);
            //if(fsc.has_changes())
            //{
            //    update_index(fsc);
            //}

            std::cout << getCurrentTime() << " processing thread " << std::this_thread::get_id()  << std::endl;
            sleep(1);
        }
    }

    /**
     * starts the background threads
     */
    void start_background_processing() {
        int disk_1 = 1;
        int disk_2 = 2;
        background_threads.emplace_back(background_thread, disk_1);
        background_threads.emplace_back(background_thread, disk_2);
    }

    /**
     * simple method to simulate work on main thread.
     * have a sleep in here to indicate work execution and finish
     */
    void main_work_execution() {
        sleep(5);
    }

public:
    int run() {
        start_background_processing();
        main_work_execution();
        //std::unique_lock<std::mutex> lk(config_mutex);
        std::cout << getCurrentTime() <<  " starting to interrupt background threads " << std::endl;
        //after main work is done interrupt the background threads
        for (unsigned i = 0; i < background_threads.size(); ++i) {
            background_threads[i].interrupt();
        }
        std::cout << getCurrentTime() <<  " waiting for background threads to join " << std::endl;
        //wait for the background threads to finish
        for (unsigned i = 0; i < background_threads.size(); ++i) {
            background_threads[i].join();
        }
        std::cout << getCurrentTime() <<  " exiting main thread " << std::endl;
    }
};


#endif //CONCURRENCY_BACKGROUND_INTERRUPTIBLE_THREADS_H