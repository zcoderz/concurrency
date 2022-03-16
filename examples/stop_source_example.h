//********************************************************
// The following code example is taken from the book
//  C++20 - The Complete Guide
//  by Nicolai M. Josuttis (www.josuttis.com)
//  http://www.cppstd20.com
//
// The code is licensed under a
//  Creative Commons Attribution 4.0 International License
//  http://creativecommons.org/licenses/by/4.0/
//********************************************************


#include <iostream>
#include <stop_token>
#include <future>    // for async()
#include <thread>    // for sleep_for()

namespace StopSourceExample {
    using namespace std::literals;  // for duration literals

    void func(std::stop_token st, int num) {
        auto id = std::this_thread::get_id();
        std::cout << "call func(" << num << ") thread id is "<< id << " \n";

        // register a first callback:
        std::stop_callback cb1{st, [num, id] {
            std::cout << "- STOP1 requested in func(" << num;
            if (id == std::this_thread::get_id()) {
                std::cout << ")-- thread id is -- " << std::this_thread::get_id() <<"\n";
            } else {
                std::cout <<  ") in main thread " << std::this_thread::get_id() <<"\n";
            }
        }};
        std::this_thread::sleep_for(9ms);

        // register a second callback:
//        std::stop_callback cb2{st, [num, id] {
//            std::cout << "- STOP2 requested in func" << num;
//            if (id == std::this_thread::get_id()) {
//                std::cout << "-- thread id is -- " << std::this_thread::get_id() <<"\n";
//            } else {
//                std::cout <<  ") in main thread " << std::this_thread::get_id() <<"\n";
//            }
//        }};
        std::this_thread::sleep_for(2ms);
    }

    void main() {
        // create stop_source and stop_token:
        std::stop_source ssrc;
        std::stop_token stok{ssrc.get_token()};
        std::cout << " main()-- thread id is -- " << std::this_thread::get_id() <<"\n" << std::flush;
        // register callback:
//        std::stop_callback cb{stok, [] {
//            std::cout << "- STOP requested in main()-- thread id is -- " << std::this_thread::get_id() <<"\n" << std::flush;
//        }};

        // in the background call func() a couple of times:
        auto fut = std::async([stok] {
            for (int num = 1; num < 10; ++num) {
                func(stok, num);
            }
        });

        // after a while, request stop:
        std::this_thread::sleep_for(10ms);
        ssrc.request_stop();
    }

}