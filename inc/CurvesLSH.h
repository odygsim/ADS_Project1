//
// Created by aikio on 22/10/19.
//

#ifndef ADS_PROJECT1_CURVESLSH_H
#define ADS_PROJECT1_CURVESLSH_H

#include <list>
#include <vector>
#include "util.h"
#include "util2.h"
#include "CurveGridHT.h"

/***** Class implementing LSH for curves ******************
 * Types :
 * D = data type of coordinates (e.g. int, double)
 * Y = data type of labels (eg string)
 * VH = type of Vector Hash (eg LSH< vector<string>, int, string > or hypercube<vector<string>, int, string >
 *
 * Main methods :
 * addCurve = adding a curve to a number of grid hashtables
 * queryCurve = querying in grids for curves of dataset with best distance (selecting dtw or other)
 *
*/
template<class D, class Y, class VH>
class CurvesLSH {

private:

    // Setting types for Curves and Points to be easily used throughout class
    typedef std::vector<std::vector<D> > CurveType;
    typedef std::vector<D> PointType;

    // Dimension of points - delta to be used ( delta =< 4*d*min{m1,m2} / m1,m2 : number of points in curves )
    // Number of grid
    // Maximum number of points in a curve to apply padding for hashing 1d vectors - assume 10% more than max points of curves in dataset
    int pointDim, numGrids, maxPointsForPadding;
    double delta, maxCoord ;
    // Parameters for LSH
    int k_vecs;
    // Parameters for Hypercube
    int k_hypercube, maxSearchPoints, probes;
    // Parameters for both
    double w;
    // Metric to be used
    std::string strMetric;
    // List of 1d HashTables (LSH or Hypercube)
    std::list<CurveGridHT<D, Y, VH> *> GridHT_ListP;

    // Pointer of the function calculating distance between curves
    std::tuple<double, std::list<std::vector<int>>> (*fDist)(CurveType &a, CurveType &b, std::string metric_name);

public:

    CurvesLSH();

    CurvesLSH(int pointDim, double delta, int kVecs, int maxPointsForPadding, int numGrids = 4, double w = 3000,
              std::string metricName = "euclidean", double maxCoord = 50);

    CurvesLSH(int pointDim, double delta, int kHypercube, int maxSearchPoints, int probes, int maxPointsForPadding,
              int numGrids = 4, double w = 3000, std::string metricName = "euclidean", double maxCoord = 50);

    // Adding a curve with label y to CurvesLSH structure
    void addX(CurveType &curve, Y &y);

    // Searching for best curve in CurvesLSH structure according to metric
    std::list<std::tuple<Y, D>> queryX(CurveType &curve);

    // Destructor : eliminates all Grid HashTable structures
    virtual ~CurvesLSH() { for (auto gHT : GridHT_ListP) delete gHT; };

};

// Constructor initializing LSH Vector Hash
template<class D, class Y, class VH>
CurvesLSH<D, Y, VH>::CurvesLSH(int pointDim, double delta, int kVecs, int maxPointsInCurves, int numGrids, double w,
                               std::string metricName, double maxCoord)
        :pointDim(pointDim), delta(delta), numGrids(numGrids), k_vecs(kVecs), strMetric(metricName),
         maxPointsForPadding(maxPointsInCurves), w(w), maxCoord(maxCoord) {

    // Set the distance metric function to be used
    fDist = &dtw<CurveType, PointType, D>;

    // Create d number of gi hypercube hashTable functions
    for (int i = 0; i < numGrids; ++i) {
        // Create the grid hash tables with LSH constructor
        GridHT_ListP.push_back(new CurveGridHT<D, Y, VH>(pointDim, delta, maxPointsForPadding, kVecs, w,
                                                         metricName, maxCoord));
    }

}

// Constructor initializing Hypercube Vector Hash
template<class D, class Y, class VH>
CurvesLSH<D, Y, VH>::CurvesLSH(int pointDim, double delta, int kHypercube, int maxSearchPoints, int probes,
                               int maxPointsInCurves,
                               int numGrids, double w, std::string metricName, double maxCoord)
        :pointDim(pointDim), delta(delta), numGrids(numGrids), k_hypercube(kHypercube),
         maxSearchPoints(maxSearchPoints), probes(probes), strMetric(metricName),
         maxPointsForPadding(maxPointsInCurves), w(w), maxCoord(maxCoord) {

    // Set the distance metric function to be used
    fDist = &dtw<CurveType, PointType, D>;

    // Create d number of gi hypercube hashTable functions
    for (int i = 0; i < numGrids; ++i) {

        // Create the grid hash tables with Hypercube constructor
        GridHT_ListP.push_back(new CurveGridHT<D, Y, VH>(pointDim, delta, maxPointsForPadding, k_hypercube,
                                                         maxSearchPoints, probes, w, metricName, maxCoord));
    }

}

// Function : Adding Curve to Grids
template<class D, class Y, class VH>
void CurvesLSH<D, Y, VH>::addX(std::vector<std::vector<D> > &curve, Y &y) {

    typename std::list<CurveGridHT<D, Y, VH> *>::iterator gridHT;

    // Add curve to all Grid HTables
    for (gridHT = GridHT_ListP.begin(); gridHT != GridHT_ListP.end(); gridHT++) {

        // Add curve to current grid
        (*gridHT)->addCurve(curve, y);

    }

}

// Function : Querying for ANN Curve in Grids
//std::list< std::tuple<Y, std::vector<std::vector<D> > > >
template<class D, class Y, class VH>
std::list<std::tuple<Y, D>> CurvesLSH<D, Y, VH>::queryX(std::vector<std::vector<D> > &curve ) {

    typename std::list<CurveGridHT<D, Y, VH> *>::iterator gridHT;
    std::list<std::tuple<Y, D>> currDistanceList, finalDistanceList, resDistList;

    // Search curve in all Grid HTables
    for (gridHT = GridHT_ListP.begin(); gridHT != GridHT_ListP.end(); gridHT++) {
        // Search curve in current grid and get list of neighbors (label, distance)
        currDistanceList = (*gridHT)->queryCurve(curve);
        // Attach to final best neighbors
        finalDistanceList.insert(finalDistanceList.end(), currDistanceList.begin(), currDistanceList.end());
    }

    // Sort all CurveGrid Hash Tables results
    finalDistanceList.sort(TupleLess<1>());
    // Return the best if any
    if ( finalDistanceList.size() > 0 ) {
        resDistList.push_back(finalDistanceList.front());
    }

    return resDistList;

}



#endif //ADS_PROJECT1_CURVESLSH_H
