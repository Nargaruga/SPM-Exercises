#include <algorithm>
#include <barrier>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

/*
 *  Exercise 3: implement in parallel the odd-even sorting algorithm. Use as input a random
 *  generated integer vector.
 */

const int SEED = 47623;
bool sorted = false;

/*
 *  Prints the contents of `vec` to stdout.
 */
void printContents(std::vector<float> vec) {
    for(size_t i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}

/*
 *  Implements a sequential version of odd-even sort to sort in-place the `vec` vector.
 */
void oddEvenSort(std::vector<float> &vec) {
    bool sorted = false;

    while(!sorted) {
        sorted = true;
        // Odd phase
        for(size_t j = 1; j < vec.size(); j += 2) {
            if(vec[j] > vec[j + 1]) {
                std::swap(vec[j], vec[j + 1]);
                sorted = false;
            }
        }

        // Even phase
        for(size_t j = 0; j < vec.size(); j += 2) {
            if(vec[j] > vec[j + 1]) {
                std::swap(vec[j], vec[j + 1]);
                sorted = false;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        std::cout << "Usage: ./map #items #workers" << std::endl;
        return -1;
    }

    srand(SEED);

    int n = atoi(argv[1]);
    std::vector<float> nums;
    std::vector<float> numsCopy;
    // Populate the input array
    for(size_t i = 0; i < n; i++) {
        float v = rand() % 10;
        nums.push_back(v);
        numsCopy.push_back(v);
    }

    std::cout << "Sorted? (before) " << std::is_sorted(nums.begin(), nums.end()) << std::endl;

    int nWorkers = atoi(argv[2]);
    std::vector<std::thread> workers(nWorkers);
    std::barrier syncPoint(nWorkers, [&] { return; });

    size_t nPairsEach = std::ceil(nums.size() / (2.0 * nWorkers));
    bool sorted = false;

    // A task to be assigned to each worker involved in performing the odd-even sort.
    auto sort = [&](int tid) {
        size_t start = tid * nPairsEach * 2;
        size_t end = std::min(nums.size(), start + nPairsEach * 2);

        while(!sorted) {
            bool localSorted = true;

            // Odd phase
            for(size_t j = start + 1; j < end; j += 2) {
                if(nums[j] > nums[j + 1] && j + 1 < nums.size()) {
                    std::swap(nums[j], nums[j + 1]);
                    localSorted = false;
                }
            }

            // Wait that all threads have completed their odd phase
            syncPoint.arrive_and_wait();
            sorted = true;

            // Even phase
            for(size_t j = start; j < end; j += 2) {
                if(nums[j] > nums[j + 1] && j + 1 < nums.size()) {
                    std::swap(nums[j], nums[j + 1]);
                    localSorted = false;
                }
            }

            if(!localSorted) {
                if(sorted) {
                    sorted = false;
                }
            }

            // Wait that all threads have completed their even phase
            syncPoint.arrive_and_wait();
        }
    };

    // Measure the running time for the sequential version
    auto seqStart = std::chrono::steady_clock::now();
    oddEvenSort(numsCopy);
    auto seqEnd = std::chrono::steady_clock::now();

    std::cout << "Sorted? (seq) " << std::is_sorted(numsCopy.begin(), numsCopy.end()) << std::endl;

    // Measure the running time for the parallel version
    auto parStart = std::chrono::steady_clock::now();
    for(size_t i = 0; i < nWorkers; i++) {
        workers[i] = std::thread(sort, i);
    }

    for(size_t i = 0; i < nWorkers; i++) {
        workers[i].join();
    }
    auto parEnd = std::chrono::steady_clock::now();

    size_t tSeq = std::chrono::duration_cast<std::chrono::microseconds>(seqEnd - seqStart).count();
    size_t tPar = std::chrono::duration_cast<std::chrono::microseconds>(parEnd - parStart).count();

    std::cout << "Sorted? (par) " << std::is_sorted(nums.begin(), nums.end()) << std::endl;

    std::cout << "Speedup: " << (float)tSeq / tPar << std::endl;
}
