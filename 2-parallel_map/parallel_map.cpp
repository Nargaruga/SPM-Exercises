#include "parallel_map.h"
#include <cmath>
#include <iostream>
#include <thread>

/*
 *  Maps the function `f` to the elements of `vec` in range [`start`, `end`).
 */
void rangeMap(std::vector<float> &vec, std::function<float(float)> f, size_t start, size_t end) {

    for(size_t i = start; i < end; i++) {
        vec[i] = f(vec[i]);
    }
}

/*
 *  Maps the function `f` to the elements of `vec` starting from `start` to the end of the array,
 *  updating every `jump`-th element.
 */
void jumpMap(std::vector<float> &vec, std::function<float(float)> f, size_t start, size_t jump) {

    for(size_t i = start; i < vec.size(); i += jump) {
        vec[i] = f(vec[i]);
    }
}

ParallelMap::ParallelMap() {}

std::vector<float> ParallelMap::parallelMap(std::vector<float> vec,
                                            SchedulingPolicy policy,
                                            std::function<float(float)> f,
                                            size_t parDegree) {

    std::vector<std::thread> workers;

    switch(policy) {
    case SchedulingPolicy::BLOCK: {
        size_t chunkSize = std::ceil((float)vec.size() / parDegree);
        size_t start = 0;
        size_t end = chunkSize;

        // Assign a different section of the vector to each thread
        for(size_t i = 0; i < parDegree; i++) {
            workers.push_back(std::thread(rangeMap, std::ref(vec), f, start, end));
            start = end;
            end = std::min(end + chunkSize, vec.size());
        }

        for(size_t i = 0; i < parDegree; i++) {
            workers[i].join();
        }

        break;
    }
    case SchedulingPolicy::CYCLIC: {
        // Each vector handles elements i + j*parDegree for j in [0..]
        for(size_t i = 0; i < parDegree; i++) {
            workers.push_back(std::thread(jumpMap, std::ref(vec), f, i, parDegree));
        }

        for(size_t i = 0; i < parDegree; i++) {
            workers[i].join();
        }

        break;
    }
    }

    return vec;
}
