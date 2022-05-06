#include <chrono>
#include <future>
#include <iostream>
#include <thread>

/*
 *  Exercise 1: evaluate the overhead needed to
 *  a) start, run and join a thread;
 *  b) start an async (launch::async policy) and get the result of the computed future.
 *
 *  Measures should be taken as averages of a large number of events.
 */

/*
 *  Dummy function
 */
int fun(int x);

/*
 *  Returns the average time spent creating, running and joining `nThreads` threads, each one
 *  calling the `fun` function.
 */
double computeThreadRuntime(int nTests, int nThreads, int (*fun)(int));

/*
 *  Returns the average time spent creating, running and waiting for the results of `nAsyncs`
 *  asyncs, each one calling the `fun` function.
 */
double computeAsyncRuntime(int nTests, int nAsyncs, int (*fun)(int));

int main(int argc, char *argv[]) {
    if(argc < 3) {
        std::cout << "Usage: ./ThreadPool #tests #workers" << std::endl;
        return -1;
    }

    int nTests = atoi(argv[1]);   // Number of tests to run
    int nWorkers = atoi(argv[2]); // Number of threads/asyncs to create

    std::cout << "[THREADS] Elapsed: " << computeThreadRuntime(nTests, nWorkers, fun)
              << " microseconds." << std::endl;

    std::cout << "[ASYNCS] Elapsed: " << computeAsyncRuntime(nTests, nWorkers, fun)
              << " microseconds." << std::endl;

    return 0;
}

int fun(int x) { return x++; }

double computeThreadRuntime(int nTests, int nThreads, int (*fun)(int)) {
    std::thread threads[nThreads];
    int64_t usec = 0;    // Running time for one thread
    int64_t usecTot = 0; // Total running time
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;

    for(size_t i = 0; i < nTests; i++) {
        start = std::chrono::system_clock::now();
        for(size_t j = 0; j < nThreads; j++) {
            threads[j] = std::thread(fun, j);
        }
        for(size_t j = 0; j < nThreads; j++) {
            threads[j].join();
        }
        end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        usec = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

        usecTot += usec;
    }

    // Return the average
    return (double)usecTot / nTests;
}

double computeAsyncRuntime(int nTests, int nAsyncs, int (*fun)(int)) {
    std::future<int> futures[nAsyncs];
    int64_t usec = 0;    // Running time for one async
    int64_t usecTot = 0; // Total running time
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;

    for(size_t i = 0; i < nTests; i++) {
        start = start = std::chrono::system_clock::now();
        for(size_t j = 0; j < nAsyncs; j++) {
            futures[j] = std::async(std::launch::async, fun, j);
        }
        for(size_t j = 0; j < nAsyncs; j++) {
            futures[j].get();
        }
        end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        usec = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

        usecTot += usec;
    }

    // Return the average
    return (double)usecTot / nTests;
}
