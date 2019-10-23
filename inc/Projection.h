//
// Created by ody on 20/10/19.
//

#ifndef ADS_PROJECT1_PROJECTION_H
#define ADS_PROJECT1_PROJECTION_H

#include <random>
#include "util.h"
#include "PathFinder.h"
#include "LSH.h"
#include "Hypercube.h"

//typedef std::vector<int> Vector; // store x, y
//typedef std::list<Vector> Path; // store list of a path
//typedef std::list<Path> Paths;  // store many paths

template<class D, class Y, class VH>
class Projection {
    // store on vector<i,j> on Paths the paths for i,j, on VH the cube or LSH class
    typedef std::tuple<Paths, VH> TupleRevPaths;
    typedef std::vector<std::vector<double>> curveX;
    std::vector<std::vector<TupleRevPaths>> ArrayTraversals;
    //    (n, vector<TupleRevPaths>(m, {MAXDOUBLE, 0, 0})); // The array m * n that hold the values calculated.
    std::vector<std::vector<double>> G; // keep G matrix here
    unsigned int d; // dimension , column limit of G
    unsigned int K; // row limit of G

    void initializer();

public:
    Projection(int d);

    // LSH constructor
    Projection(int d, int w = 6000, int k = 4, int L = 5, int m = 0, double radius = 0, int top_limit = 0,
               std::string metric_name = "manhattan");

    void addX(curveX &x, Y &y);  // Add point to structure.

    std::list<std::tuple<Y, D>> queryX(CurveX &x) const; // Query a Point it return a list of tuples (label, distance).
    // Hypercube constructor
    Projection(int d, double w = 3000, int k = 3, int maxSearchPoints = 10, int probes = 2, int k_hi = 4, double r = 0);
};


//LSH(int d, int w = 6000, int k = 4, int L = 5, int m = 0, double radius = 0, int top_limit = 0,
//    std::string metric_name = "manhattan"); // Constructor 1.
template<class D, class Y, class VH>
Projection<D, Y, VH>::Projection(int d, int w, int k, int L, int m, double radius, int top_limit,
                                 std::string metric_name) {

    // create the Array MXM // TODO check if i need to start from 1.
    for (int i = 0; i < d; ++i) {
        for (int j = 0; j < d; ++j) {
            // Get Relevant Paths for i,j
            PathFinder *pathFinder = new PathFinder(i, j);
            Paths paths = pathFinder->RelevantPaths();
            delete pathFinder;
            ArrayTraversals.push_back({ paths, new LSH<Vector, D, Y>(d, w, k, L, m, radius, top_limit, metric_name)
                                        });
        }
    }
    // calculate relevant traversals for each ij of MxM and store them in ArrayTraversals

}

template<class D, class Y, class VH>
Projection<D, Y, VH>::Projection(int d, double w, int k, int maxSearchPoints, int probes, int k_hi, double r){

    // create the Array MXM
    for (int i = 0; i < d; ++i) {
        for (int j = 0; j < d; ++j) {
            // Get Relevant Paths for i,j
            PathFinder *pathFinder = new PathFinder(i, j);
            Paths paths = pathFinder->RelevantPaths();
            delete pathFinder;
            ArrayTraversals.push_back({paths, new Hypercube<Vector, D, Y>(d, w, k, maxSearchPoints, probes, k_hi, r)
                                       });
        }
    }
    // calculate relevant traversals for each ij of MxM and store them in ArrayTraversals
}
#endif //ADS_PROJECT1_PROJECTION_H
