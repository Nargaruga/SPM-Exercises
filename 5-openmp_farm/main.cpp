#include "../timer.h"
#include <iostream>
#include <omp.h>

/*
 * Performs an active wait for `target` milliseconds.
 */
void activewait(long target) {
    auto start = std::chrono::high_resolution_clock::now();
    auto end = false;
    while(!end) {
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        auto elapsedCount = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        if(elapsedCount > target)
            end = true;
    }
    return;
}

int main(int argc, char *argv[]) {
    if(argc < 5) {
        std::cout << "Usage: ./farm #workers wait_t (in ms) task_t (in ms) #tasks" << std::endl;
        return -1;
    }

    int nWorkers = atoi(argv[1]); // Number of worker threads
    int wait_t = atoi(argv[2]);   // Time between tasks
    int task_t = atoi(argv[3]);   // Time to perform a (simulated) task
    int nTasks = atoi(argv[4]);   // Number of tasks
    if(nWorkers <= 0 || wait_t <= 0 || task_t <= 0 || nTasks <= 0) {
        std::cout << "None of the parameters can be <= 0." << std::endl;
        return -1;
    }

    double start, end;
    start = omp_get_wtime();
#pragma omp parallel num_threads(nWorkers)
    {
#pragma omp single
        {
            for(size_t i = 0; i < nTasks; i++) {
#pragma omp task
                activewait(task_t); // Simulate execution of the task
                activewait(wait_t); // Simulate the wait for the next task
            }
        }
    }
    end = omp_get_wtime();

    std::cout << "Done in " << end - start << "." << std::endl;
}