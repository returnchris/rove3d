#pragma once
///
#include <thread>
#include <atomic>
#include "utility.h"
///
struct Thread {
  uint32_t id;
  uint32_t begin;
  uint32_t end;
  std::atomic<int32_t> stop;
  std::atomic<int32_t> work_available;
  std::function<void(uint32_t, uint32_t, uint32_t)> work;
  std::thread thread;
};
///
struct ThreadPool {
  ///
  void Setup() 
  {
    LOG("Setting up thread pool.");
    m_threads = (Thread*)malloc(sizeof(Thread) * (ROVE.thread_count - 1));
    for (uint32_t t = 0; t < (ROVE.thread_count - 1); ++t) {
      Thread* thread = &m_threads[t];
      thread->id = t;
      new (&thread->stop) std::atomic<int32_t>(false);
      new (&thread->work_available) std::atomic<int32_t>(false);
      new (&thread->work) std::function<void(uint32_t thread_id,
                                             uint32_t thread_begin,
                                             uint32_t thread_end)>();
      new (&thread->thread) std::thread(
        [t, this] {
          Thread* thread = &m_threads[t];
          while (!thread->stop) {
            if (!thread->work_available) {
              std::this_thread::yield();
            } else {
              thread->work(thread->id, thread->begin, thread->end);
              thread->work_available = false;
            }
          }
        });
    }
  }
  ///
  void Shutdown()
  {
    LOG("Shutting down thread pool.");
    if (m_threads) {
      for (uint32_t t = 0; t < ROVE.thread_count - 1; ++t) {
        Thread* thread = &m_threads[t];
        thread->stop = true;
        if (thread->thread.joinable()) {
          thread->thread.join();
        }
      }
      free(m_threads);
    }
  }
  ///
  #define PARALLEL(ITEM_COUNT, FUNCTION) \
  { \
    uint32_t leftover = ITEM_COUNT % ROVE.thread_count; \
    uint32_t base_count = ITEM_COUNT / ROVE.thread_count; \
    uint32_t thread_id; \
    for (thread_id = 0; thread_id < ROVE.thread_count; ++thread_id) { \
      uint32_t thread_begin; \
      uint32_t thread_end; \
      if (thread_id < leftover) { \
        uint32_t count = base_count + 1; \
        thread_begin = count * thread_id; \
        thread_end = thread_begin + count; \
      } else { \
        thread_begin = (base_count * thread_id) + leftover; \
        thread_end = thread_begin + base_count; \
      } \
      if (thread_id == (ROVE.thread_count - 1)) { \
        FUNCTION; \
      } else {\
        Thread* thread = &THREAD_POOL->m_threads[thread_id]; \
        thread->begin = thread_begin; \
        thread->end = thread_end; \
        thread->work = [&](uint32_t thread_id, \
                           uint32_t thread_begin, \
                           uint32_t thread_end) FUNCTION; \
        thread->work_available = true; \
      } \
    } \
    for (uint32_t t = 0; t < ROVE.thread_count - 1; ++t) { \
      Thread* thread = &THREAD_POOL->m_threads[t]; \
      while (thread->work_available) { \
        std::this_thread::yield(); \
      } \
    } \
  }
  Thread* m_threads;
};
