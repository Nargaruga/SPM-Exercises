#include "../timer.h"
#include "parallel_map.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

/*
 *  Exercise 2: provide a vector<float> map(vector<float>v, int mode, function<float(float)> f, int
 *  pardegree) computing in parallel (using C++ threads or asyncs) the map(f) over v. Mode is 0
 *  (block) or 1 (cyclic) scheduling.
 */

const int SEED = 67238;

void activewait(long target);
int f(int x);
void printContents(std::vector<float> vec);

int main(int argc, char *argv[]) {
    if(argc < 2) {
        std::cout << "Usage: ./map #items" << std::endl;
        return -1;
    }

    ParallelMap pMap;

    int n = atoi(argv[1]);
    std::vector<float> nums;
    srand(SEED);
    // Populate the input array with random numbers
    for(size_t i = 0; i < n; i++) {
        nums.push_back(std::rand() % 10);
    }

    {
        Timer timer("[BLOCK] One thread", Timer::Mode::MILLISECONDS);
        pMap.parallelMap(nums, ParallelMap::SchedulingPolicy::BLOCK, &f, 1);
    }

    {
        Timer timer("[BLOCK] Four threads", Timer::Mode::MILLISECONDS);
        pMap.parallelMap(nums, ParallelMap::SchedulingPolicy::BLOCK, &f, 4);
    }

    {
        Timer timer("[CYCLIC] One thread", Timer::Mode::MILLISECONDS);
        pMap.parallelMap(nums, ParallelMap::SchedulingPolicy::CYCLIC, &f, 1);
    }

    {
        Timer timer("[CYCLIC] Four threads", Timer::Mode::MILLISECONDS);
        pMap.parallelMap(nums, ParallelMap::SchedulingPolicy::CYCLIC, &f, 4);
    }
}

// Performs busy waiting for `target` milliseconds
void activewait(int64_t target) {
    auto start = std::chrono::high_resolution_clock::now();
    auto done = false;
    while(!done) {
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        int64_t msec = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        if(msec > target)
            done = true;
    }
    return;
}

// Dummy function which fakes a long computation
int f(int x) {
    activewait(100);
    return x + x;
}

// Prints the contents of `vec` to stdout
void printContents(std::vector<float> vec) {
    for(size_t i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << std::endl;
    }
}