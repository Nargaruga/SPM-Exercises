#include "threadPool.h"
#include <functional>
#include <future>
#include <iostream>
#include <vector>

/*
 *  Exercise 4: implement a task pool using C++ threads and STDLIB. It must be possible to specify
 *  the parallelism degree as parameter and tasks should either produce results through side effects
 *  or in such a way that they can be retrieved by the task emitter.
 */

/*
 *  Dummy function.
 */
int test(int x) { return x + x; }

int main(int argc, char *argv[]) {
    if(argc < 3) {
        std::cout << "Usage: ./ThreadPool #threads #tasks" << std::endl;
        return -1;
    }

    int nThreads = atoi(argv[1]);
    int nTasks = atoi(argv[2]);
    ThreadPool<int> pool(nThreads);
    std::vector<std::future<int>> futures;

    // Prepare the tasks to be executed and insert them in the pool's queue
    for(int i = 0; i < nTasks; i++) {
        std::packaged_task<int()> task(std::bind(test, i));
        futures.push_back(task.get_future());
        pool.insertTask(std::move(task));
    }

    pool.conclude();

    // Dummy sum computed for testing purposes
    int sum = 0;
    std::cout << "Result:" << std::endl;
    for(int i = 0; i < nTasks; i++) {
        sum += futures[i].get();
    }
    std::cout << sum << std::endl;
}