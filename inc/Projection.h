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
    typedef std::vector<D> PointType, ConcatVector;
    typedef std::vector<PointType> CurveType;            // curveX
    typedef std::tuple<CurveType, Y> curveDataType;
    typedef std::tuple<Path, VH *> TupleRelevantPath;     // Path, VH
    typedef std::vector<TupleRelevantPath> RelevantPaths;       // vector<{Path, VH}>

    std::vector<std::vector<RelevantPaths>> ArrayTraversals;    // The array of Traversals MxM with objects RelevantPaths

    // Setting type for LSH, Hypercube
    typedef LSH<PointType, D, curveDataType> LSHType;
    typedef Hypercube<PointType, D, curveDataType> HQType;

    unsigned int dimensionPoint, dimensionCurve; // dimension , column limit of G
    unsigned int K, window = 2; // row limit of G
    double e;
    std::string metric_name;

//    std::tuple<D, std::list<std::vector<int>>> (*f)(CurveType &, CurveType &, std::string);
    D (*f)(CurveType &, CurveType &, std::string);

    CurveType G;// keep G matrix here
    std::vector<D> mulCurveAndG(CurveType x);

    std::list<std::tuple<Y, D>> scanTraversals(CurveType &, int w = 2);

    std::string name = "ANNProjection";

    void initializer();

public:
    Projection(int d);

    std::string getName() const { return name; }

    // LSH constructor
    Projection(int dimensionCurve, int dimensionPoint, int w = 6000, int k = 4, int L = 5, int m = 0,
               double radius = 0,
               int top_limit = 0, double e = 0.5, std::string metric_name = "euclidean");

    void addX(CurveType &x, Y &y);  // Add point to structure.

    std::list<std::tuple<Y, D>> queryX(CurveType &x); // Query a Point it return a list of tuples (label, distance).
    // Hypercube constructor
    Projection(int dimensionCurve, int dimensionPoint, double w = 5000, int k_hypercube = 3, int maxSearchPoints = 10,
               int probes = 2, double r = DBL_MAX, double e = 0.5, std::string metric_name = "euclidean");
//    Projection(int dimensionCurve, int dimensionPoint, double w = 3000, int k = 3, int maxSearchPoints = 10,
//               int probes = 2, int k_hi = 4, double r = 0, std::string metric_name = "euclidean");

};


//LSH(int d, int w = 6000, int k = 4, int L = 5, int m = 0, double radius = 0, int top_limit = 0,
//    std::string metric_name = "manhattan"); // Constructor 1.
template<class D, class Y, class VH>
Projection<D, Y, VH>::Projection(int dimensionCurve, int dimensionPoint, int w, int k, int L, int m,
                                 double radius,
                                 int top_limit, double e, std::string metric_name):metric_name(metric_name),
                                                                                   dimensionCurve(dimensionCurve),
                                                                                   dimensionPoint(dimensionPoint),
                                                                                   e(e) {

    initializer();
    f = &dtwD<CurveType, PointType, D>;
    // create the Array MXM // TODO check if i need to start from 1.
    for (int i = 0; i < dimensionCurve; ++i) {
        std::vector<std::vector<TupleRelevantPath>> rowTuple;
        for (int j = 0; j < dimensionCurve; ++j) {
            // Get Relevant Paths for i,j
            PathFinder *pathFinder = new PathFinder(i, j, window);
            Paths paths = pathFinder->RelevantPaths();
            delete pathFinder;
            RelevantPaths relPathsVector;
            for (auto path: paths) { // const &?
                // add to each path a Vector Hashtable lsh/cube
                TupleRelevantPath t = std::make_tuple(path,
                                                      new LSHType(dimensionPoint, w, k, L, m, radius, top_limit,
                                                                  metric_name));
                relPathsVector.push_back(t);
            }
            rowTuple.push_back(relPathsVector);
        }
        ArrayTraversals.push_back(rowTuple);
    }
    // calculate relevant traversals for each ij of MxM and store them in ArrayTraversals

}

/// HYPERCUBE
template<class D, class Y, class VH>
Projection<D, Y, VH>::Projection(int dimensionCurve, int dimensionPoint, double w, int k_hypercube, int maxSearchPoints,
                                 int probes, double r, double e, std::string metric_name):metric_name(metric_name),
                                                                                          e(e), dimensionCurve(
                dimensionCurve), dimensionPoint(dimensionPoint) {
    e = 0.5;
    int k_hi = 4;
    initializer();
    f = &dtwD<CurveType, PointType, D>;
    // create the Array MXM
    for (int i = 0; i < dimensionCurve; ++i) {
        std::vector<std::vector<TupleRelevantPath>> rowTuple;
        for (int j = 0; j < dimensionCurve; ++j) {
            // Get Relevant Paths for i,j
//            if ( i == 5 && j == 5)
//                std::cout<< "a\n" ;
            PathFinder *pathFinder = new PathFinder(i, j, window);
            Paths paths = pathFinder->RelevantPaths();
            delete pathFinder;
            RelevantPaths relPathsVector;
            for (auto path: paths) { // const &?
                // add to each path a Vector Hashtable lsh/cube
                TupleRelevantPath t = std::make_tuple(path,
                                                      new HQType(dimensionPoint, w, k_hypercube, maxSearchPoints,
                                                                 probes, k_hi,
                                                                 r));
                relPathsVector.push_back(t);
            }
            rowTuple.push_back(relPathsVector);
        }
        ArrayTraversals.push_back(rowTuple);
    }
}


template<class D, class Y, class VH>
// Add point to structure.
void Projection<D, Y, VH>::addX(CurveType &x, Y &y) {
    int numPoints = x.size() - 1;
    //point for : a) concatenation of grid points
    ConcatVector hashedX;

    // Concatenate grid curve points to a vector
    hashedX = mulCurveAndG(x);

    // Make a tuple of (pointer to curve , curve label)
    curveDataType curveData = std::make_tuple(x, y);
    // Get the relevant paths to add the point
    RelevantPaths &cur = ArrayTraversals[numPoints][numPoints];
    for (auto tup: cur) {         // for each path, VH
        VH *vh = std::get<1>(tup);
        vh->addX(hashedX, curveData);       // add the point hashed vector and the labelwith the curve.
    }
}

template<class D, class Y, class VH>
// Query a Point it return a list of tuples (label, distance).
std::list<std::tuple<Y, D>> Projection<D, Y, VH>::scanTraversals(CurveType &x, int w) {

    typedef std::list<std::tuple<curveDataType, D>> listTuples;                 // Declare Types list of tuples.
    typedef std::list<std::tuple<Y, D>> listRetTuples;                 // Declare Types list of tuples.
    typedef typename listTuples::iterator IteratorListTuples;     // Declare Iterator of list of tuples.
    typedef typename listRetTuples::iterator IteratorListRetTuples;     // Declare Iterator of list of tuples.
    curveDataType currCurveData;
    CurveType currCurve;
    Y currCurveLabel = "";
    Y bestCurveLabel = "";
    double bestDist = DBL_MAX;
    double currDist = 0;

//    std::tuple<double, std::list<std::vector<int>>> (*f)(CurveType &, CurveType &, std::string);
    D (*f)(CurveType &, CurveType &, std::string) = &dtwD<CurveType, std::vector<D>, D>;

    int numPoints = x.size() - 1;
    int currI, currJ;
    std::vector<double> hashedX;

    listTuples currDistanceList, finalDistanceList;                                      // List that stores the distances from neighbors.
    listRetTuples labelDistanceRetList, retList;                              // List that stores the labels of neighbors.
    IteratorListTuples iterListTuples;                            // iterator to traverse the list.

    hashedX = mulCurveAndG(x); // mul x * G

    std::list<std::vector<int>> Actions{
            {0,  0}, // current
            {0,  1},
            {1,  0},
            {0,  -1},
            {-1, 0}
    };        // a list of action N, S, E, W
    if (w == 1) {
        Actions.pop_back();
        Actions.pop_back();
    }

    for (auto action: Actions) {     // for all actions gather results of queries
        // Get neighbors or current relevant path
        currI = numPoints + action[0];
        currJ = numPoints + action[1];
        // if position is not out of limits
        if (currI < dimensionCurve && currJ < dimensionCurve) {
            RelevantPaths &cur = ArrayTraversals[currI][currJ];
            for (auto tup: cur) {         // for each each Traversal aka path, VH
                VH *vh = std::get<1>(tup);      // get ANN
                currDistanceList = vh->queryX(hashedX);       // query the ANN  and append the results to a list.
                // Attach to final best neighbors
                finalDistanceList.insert(finalDistanceList.end(), currDistanceList.begin(), currDistanceList.end());
            }
        }
    }
    finalDistanceList.sort(TupleLess<1>()); // sort by neighbors
/// TODO check if i need to run dtw on the results.
    // iterator for traverse the list.
    for (iterListTuples = finalDistanceList.begin(); (iterListTuples != finalDistanceList.end()); ++iterListTuples) {

        // this a tuple of (y, distanceFromDtw)
        // extact y string and curve
        currCurveData = std::get<0>(*iterListTuples);
        // Getting curve from curve data tuple
        currCurve = std::get<0>(currCurveData);
        // Getting curve Label from curve data tuple
        currCurveLabel = std::get<1>(currCurveData);
        // Calculate the distance from current Curve and queryCurve
        currDist = f(currCurve, x, metric_name);
        // push the result to a list to be sorted to the best neighbor
        labelDistanceRetList.push_back(std::make_tuple(currCurveLabel, currDist));
    }

    if (!labelDistanceRetList.empty()) {

        labelDistanceRetList.sort(TupleLess<1>()); // sort by neighbors
        // Now append the nearest neighbors
        retList.push_back(labelDistanceRetList.front());
    }
    return retList;
}

template<class D, class Y, class VH>
// Query a Point it return a list of tuples (label, distance).
std::list<std::tuple<Y, D>> Projection<D, Y, VH>::queryX(CurveType &x) {

    return scanTraversals(x);  // scan and return result
}

template<class D, class Y, class VH>
// Query a Point it return a list of tuples (label, distance).
std::vector<D> Projection<D, Y, VH>::mulCurveAndG(CurveType x) {
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
    f = &dtwD<CurveType, PointType, D>;
    K = KCalculation(e, dimensionPoint);
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
