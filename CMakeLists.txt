cmake_minimum_required(VERSION 3.21)
project(concurrency)
set(Boost_INCLUDE_DIR /home/usman/boost/boost_1_77_0)
set(Boost_LIBRARY_DIR /home/usman/boost/boost_1_77_0/stage/lib)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXE_LINKER_FLAGS "-pthread  -lpthread")

add_executable(concurrency main.cpp exercises/odd_even_runs.cpp exercises/odd_even_runs.h exercises/simple_mutex_and_lock.cpp exercises/simple_mutex_and_lock.h containers/thread_safe_stack.h utils/hierarchical_mutex.h examples/shared_lock.h examples/combined_locks_swap.h examples/call_once.h examples/packaged_task.h examples/asynch_example.h examples/wait_until.h examples/future_example.h examples/promise_example.h examples/barrier_example.h examples/latch_example.h examples/parallel_quick_sort.h examples/parallel_for_each.h examples/parallel_find.h containers/thread_safe_lookup_table.h utils/function_wrapper.h containers/thread_safe_queue.h utils/thread_pool.h containers/work_stealing_queue.h utils/thread_joinable.h utils/spin_lock_mutex.h examples/memory_order/atomic_example.h examples/memory_order/release_acquire_example.h examples/memory_order/multi_thread_release_acquire.h examples/memory_order/producer_consumer_with_fetch_sub.h examples/parallel_partial_sum.h examples/parallel_page_visits.h examples/multi_threaded_testing.h utils/interruptible_flag.h examples/background_interruptible_threads.h examples/parallel_for_each_two.h examples/timer.h examples/parallel_transform_reduce.h examples/atomic_stack.h)

include_directories(${Boost_INCLUDE_DIR})