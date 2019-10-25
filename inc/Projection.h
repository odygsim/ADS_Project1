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
    /**
     * @brief Each cell of MXM has the type ReleventPaths, which is a vector of <{Path,VH}>. Meaning
     * each cell has many Relevant paths that why a vector was chosen.
     */
    // store on vector<i,j> on Paths the paths for i,j, on VH the cube or LSH class
    typedef std::tuple<Path, VH> TupleReleventPath;     // Path, VH
    typedef std::vector<TupleReleventPath> RelevantPaths;       // vector<{Path, VH}>
    typedef std::vector<std::vector<double>> curveX;            // curveX
    std::vector<std::vector<RelevantPaths>> ArrayTraversals;    // The array of Traversals MxM
    typedef std::tuple<std::string, std::vector<std::vector<double>> &> Y_Curve_ANN;
    //    (n, vector<TupleRevPaths>(m, {MAXDOUBLE, 0, 0})); // The array m * n that hold the values calculated.
//    std::vector<std::vector<double>> G;
    unsigned int dimensionPoint, dimensionCurve; // dimension , column limit of G
    unsigned int K, window = 2; // row limit of G
    std::string metric_name;

    std::tuple<double, std::list<std::vector<int>>> (*f)(curveX &, curveX &, std::string);

    curveX G;// keep G matrix here
//    std::vector<double> mulCurveAndG(curveX &);
    std::vector<double> mulCurveAndG(curveX x);

    std::list<std::tuple<Y, D>> scanTraversals(curveX &, int w = 2);

    void initializer();

public:
    Projection(int d);

    // LSH constructor
    Projection(int dimensionCurve, int dimensionPoint, int w = 6000, int k = 4, int L = 5, int m = 0, double radius = 0,
               int top_limit = 0, std::string metric_name = "euclidean");

    void addX(curveX &x, Y &y);  // Add point to structure.

    std::list<std::tuple<Y, D>>
    queryX(curveX &x) const; // Query a Point it return a list of tuples (label, distance).
    // Hypercube constructor
    Projection(int dimensionCurve, int dimensionPoint, double w = 3000, int k = 3, int maxSearchPoints = 10,
               int probes = 2, int k_hi = 4, double r = 0, std::string metric_name = "euclidean");

};


//LSH(int d, int w = 6000, int k = 4, int L = 5, int m = 0, double radius = 0, int top_limit = 0,
//    std::string metric_name = "manhattan"); // Constructor 1.
template<class D, class Y, class VH>
Projection<D, Y, VH>::Projection(int dimensionCurve, int dimensionPoint, int w, int k, int L, int m, double radius,
                                 int top_limit, std::string metric_name):metric_name(metric_name) {

    initializer();
    // create the Array MXM // TODO check if i need to start from 1.
    for (int i = 0; i < dimensionCurve; ++i) {
        for (int j = 0; j < dimensionCurve; ++j) {
            // Get Relevant Paths for i,j
            PathFinder *pathFinder = new PathFinder(i, j, window);
            Paths paths = pathFinder->RelevantPaths();
            delete pathFinder;
            RelevantPaths relPathsVector;
            for (auto path: paths) { // const &?
                // add to each path a Vector Hashtable lsh/cube
                relPathsVector.push_back({path,
                                          new LSH<Vector, D, Y_Curve_ANN>(dimensionPoint, w, k, L, m, radius, top_limit,
                                                                          metric_name)});
            }
            ArrayTraversals.push_back({relPathsVector});
        }
    }
    // calculate relevant traversals for each ij of MxM and store them in ArrayTraversals

}

template<class D, class Y, class VH>
Projection<D, Y, VH>::Projection(int dimensionCurve, int dimensionPoint, double w, int k, int maxSearchPoints,
                                 int probes, int k_hi, double r, std::string metric_name):metric_name(metric_name) {

    initializer();
    // create the Array MXM
    for (int i = 0; i < dimensionPoint; ++i) {
        for (int j = 0; j < dimensionPoint; ++j) {
            // Get Relevant Paths for i,j
            PathFinder *pathFinder = new PathFinder(i, window);
            Paths paths = pathFinder->RelevantPaths();
            delete pathFinder;
            ArrayTraversals.push_back({paths,
                                       new Hypercube<Vector, D, Y_Curve_ANN>(dimensionPoint, w, k, maxSearchPoints,
                                                                             probes, k_hi, r)});
        }
    }
    // calculate relevant traversals for each ij of MxM and store them in ArrayTraversals
}


template<class D, class Y, class VH>
// Add point to structure.
void Projection<D, Y, VH>::addX(curveX &x, Y &y) {
    int numPoints = x.size();
    std::vector<double> hashedX;
    hashedX = mulCurveAndG(x);
//    typedef std::vector<TupleReleventPath> RelevantPaths;       // vector<{Path, VH}>
    RelevantPaths &cur = ArrayTraversals[numPoints][numPoints];
    for (auto tup: cur) {         // for each path, VH
        VH &vh = std::get<1>(tup);
        vh.addPoint(hashedX, {y, x});       // add the point hashed vector and the labelwith the curve.
    }
}

template<class D, class Y, class VH>
// Query a Point it return a list of tuples (label, distance).
std::list<std::tuple<Y, D>> Projection<D, Y, VH>::scanTraversals(curveX &x, int w) {

    typedef std::list<std::tuple<Y_Curve_ANN, D>> listTuples;                 // Declare Types list of tuples.
    typedef std::list<std::tuple<Y, D>> listRetTuples;                 // Declare Types list of tuples.
    typedef typename listTuples::iterator IteratorListTuples;     // Declare Iterator of list of tuples.
    typedef typename listRetTuples::iterator IteratorListRetTuples;     // Declare Iterator of list of tuples.

    std::tuple<double, std::list<std::vector<int>>> (*f)(curveX &, curveX &, std::string);
    f = &dtw<curveX, std::vector<double>, double>;

    int numPoints = x.size();
    std::vector<double> hashedX;

    listTuples distanceList;                                      // List that stores the distances from neighbors.
    listRetTuples labelDistanceRetList, retList;                              // List that stores the labels of neighbors.
    IteratorListTuples iterListTuples;                            // iterator to traverse the list.

    hashedX = mulCurveAndG(x); // mul x * G

    std::list<std::vector<int>> Actions{{0,  1},
                                        {1,  0},
                                        {0,  -1},
                                        {-1, 0}};        // a list of action N, S, E, W
    if (w == 1) {
        Actions.pop_back();
        Actions.pop_back();
    }
    for (auto action: Actions) {     // for all actions gather results of queries
        RelevantPaths &cur = ArrayTraversals[numPoints + action[0]][numPoints + action[1]];
        for (auto tup: cur) {         // for each each Traversal aka path, VH
            VH &vh = std::get<1>(tup);      // get ANN
            distanceList.push_back(vh.queryPoint(hashedX));       // query the ANN  and append the results to a list.
        }
    }
    distanceList.sort(TupleLess<1>()); // sort by neighbors
/// TODO check if i need to run dtw on the results.
    IteratorListTuples itE = distanceList.end(); // iterator for traverse the list.
    for (iterListTuples = distanceList.begin(); (iterListTuples != itE); ++iterListTuples) {

        // this a tuple of (y, distanceFromDtw)
        labelDistanceRetList.push_back({std::get<0>(*iterListTuples), f(std::get<1>(std::get<0>(*iterListTuples)), x, metric_name)});
    }

    labelDistanceRetList.sort(TupleLess<1>()); // sort by neighbors
    // Now append the nearest neighbors
    retList.push_back(labelDistanceRetList.front());
    return labelDistanceRetList;
}

template<class D, class Y, class VH>
// Query a Point it return a list of tuples (label, distance).
std::list<std::tuple<Y, D>> Projection<D, Y, VH>::queryX(curveX &x) const {

    return scanTraversals(x);  // scan and return result
}

template<class D, class Y, class VH>
// Query a Point it return a list of tuples (label, distance).
std::vector<double> Projection<D, Y, VH>::mulCurveAndG(curveX x) {
    std::vector<double> rowX;
    std::vector<double> rowY;
    double sum = 0;

    int numPoints = x.size();
    for (int l = 0; l < numPoints; l++) {  // multiply curve points with G
        std::vector<double> currentPoint = x[l];   // get each point

        for (int j = 0; j < dimensionPoint; ++j) {  // for each column of G sum each column value * point
            for (int i = 0; i < K; ++i) {   // now for each row of G
                sum += currentPoint[j] * G[i][j];
            }
            if (j == 0)                      // aka xi 's
                rowX.push_back(sum);
            else                            // aka yi 's
                rowY.push_back(sum);
            sum = 0;
        }
    }
    // append yi's to xi's to create the concatenated vector that's going to be hashed.
    for (const auto d: rowY) { rowX.push_back(d); }
    return rowX;
}

template<class D, class Y, class VH>
// Init G
void Projection<D, Y, VH>::initializer() {
    f = &dtw<curveX, std::vector<double>, double>;
    std::random_device randDev;
    std::mt19937 generator(randDev());
    std::normal_distribution<double> distribution(0.0, 1.0);
    for (int i = 0; i < K; ++i) {
        std::vector<double> row;
        for (int j = 0; j < dimensionPoint; ++j) {
            row.push_back(distribution(generator));
        }
        G.push_back(row);
    }
}

#endif //ADS_PROJECT1_PROJECTION_H
