//
// Created by usman on 3/16/22.
//

#ifndef CONCURRENCY_ATOMIC_STACK_H
#define CONCURRENCY_ATOMIC_STACK_H

#include <atomic>
#include <thread>
#include <vector>
#include <iostream>
namespace StackExample {
    using namespace std::literals;  // for duration literals
    template<typename T>
    struct node {
        T data;
        node *next;

        node(const T &data) : data(data), next(nullptr) {}
    };

    template<typename T>
    class StackLockFree {
        std::atomic<node<T> *> head;
    public:
        void push(const T &data) {
            node<T> *new_node = new node<T>(data);

            // put the current value of head into new_node->next
            new_node->next = head.load(std::memory_order_relaxed);

            // now make new_node the new head, but if the head
            // is no longer what's stored in new_node->next
            // (some other thread must have inserted a node just now)
            // then put that new head into new_node->next and try again
            while (!head.compare_exchange_weak(new_node->next, new_node,
                                               std::memory_order_release,
                                               std::memory_order_relaxed)); // the body of the loop is empty
        }

        void print() const {
            std::cout << "HEAD";
            for (auto p = head.load(); p; p = p->next) {  // atomic read
                std::cout << "->" << p->data;
            }
            std::cout << std::endl;
        }
    };

    void main() {
        StackLockFree<std::string> aStack;

        // populate list by elements from 10 threads:
        {
            std::vector<std::jthread> threads;
            for (int i = 0; i < 100; ++i) {
                threads.push_back(std::jthread{[&, i]{
                    for (auto s : {"hi", "hey", "ho", "last"}) {
                        aStack.push(std::to_string(i) + s);
                        std::this_thread::sleep_for(5ns);
                    }
                }});
            }
        } // wait for all treads to finish

        // print resulting list:
        aStack.print();
    }

}
#endif //CONCURRENCY_ATOMIC_STACK_H
