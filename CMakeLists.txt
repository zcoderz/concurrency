cmake_minimum_required(VERSION 3.21)
project(concurrency)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXE_LINKER_FLAGS "-pthread  -lpthread")

add_executable(concurrency main.cpp exercises/OddEvenRuns.cpp exercises/OddEvenRuns.h exercises/SimpleMutexAndLock.cpp exercises/SimpleMutexAndLock.h containers/thread_safe_stack.h utils/hierarchical_mutex.h examples/shared_lock.h examples/combined_locks_swap.h examples/call_once.h containers/thread_safe_queue.h examples/packaged_task.h examples/asynch_example.h examples/wait_until.h examples/future_example.h examples/promise_example.h examples/barrier_example.h examples/latch_example.h examples/parallel_quick_sort.h examples/parallel_for_each.h examples/parallel_find.h containers/thread_safe_lookup_table.h)
