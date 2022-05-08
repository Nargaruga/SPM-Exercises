#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <condition_variable>
#include <deque>
#include <future>
#include <iostream>
#include <mutex>
#include <syncstream>
#include <vector>

/*
 *   Class that implements a thread pool.
 *   A queue handles tasks in a FIFO fashion, with insertion
 *   done externally through a public interface and extraction done
 *   by the threads of the pool.
 */
template <class R> class ThreadPool {
  public:
    /*
     *   Creates a thread pool with `nThreads` workers.
     */
    ThreadPool(int nThreads) {
        if(nThreads < 1)
            return;

        m_nWorkers = nThreads;

        for(size_t i = 0; i < m_nWorkers; i++) {
            m_workers.push_back(std::thread(&ThreadPool::doWork, this, i));
        }
    };

    /*
     *  Inserts `task` in the task queue and wakes a worker.
     */
    void insertTask(std::packaged_task<R()> task) {
        std::scoped_lock(m_mtx);
        m_tasks.push_back(std::move(task));
        m_cond.notify_one();
    };

    /*
     *  Thread `tid` keeps extracting end executing tasks
     *  until the thread pool closes.
     */
    void doWork(size_t tid) {
        {
            std::osyncstream syncstream(std::cout);
            syncstream << "Thread " << tid << " started." << std::endl;
        }

        while(true) {
            std::unique_lock<std::mutex> lck(m_mtx);
            while(m_tasks.empty()) {
                if(m_stop) {
                    // There are no tasks left and the thread pool is trying to close,
                    // we should return.
                    lck.unlock();
                    return;
                }

                m_cond.wait(lck);
            }

            // Get a task and execute it
            auto task = std::move(m_tasks.front());
            m_tasks.pop_front();
            lck.unlock();

            task();
        }
    }

    /*
     * Prepares to close the thread pool.
     */
    void conclude() {
        m_stop = true;
        m_cond.notify_all();
        std::osyncstream syncstream(std::cout);
        for(size_t i = 0; i < m_nWorkers; i++) {
            m_workers[i].join();
            syncstream << "Thread " << i << " stopped." << std::endl;
        }
    };

  private:
    std::atomic<bool> m_stop = false; // When true, the threadpool will close
    int m_nWorkers;                   // Number of workers in the pool
    std::vector<std::thread> m_workers;
    std::deque<std::packaged_task<R()>> m_tasks;
    std::mutex m_mtx;               // Mutex for synchronized access to the task queue
    std::condition_variable m_cond; // Needed to avoid active wait when the queue is empty
};

#endif
