#ifndef PARALLEL_MAP_H
#define PARALLEL_MAP_H

#include <functional>
#include <vector>

/*
 *  Class that implements the map pattern with customizable
 *  scheduling policy and parallelization degree.
 */
class ParallelMap {

  public:
    enum SchedulingPolicy { BLOCK, CYCLIC };

    /*
     *  Empty constructor
     */
    ParallelMap();

    /*  Maps the function `f` to all the elements of `vec` with a parallelism
     *  policy defined by the `policy` and `parDegree` parameters.
     *  Returns the appropriately updated copy of vec.
     */
    std::vector<float> parallelMap(std::vector<float> vec,
                                   SchedulingPolicy policy,
                                   std::function<float(float)> f,
                                   size_t parDegree);
};

#endif