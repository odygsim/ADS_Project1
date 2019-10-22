//
// Created by aikio on 22/10/19.
//

#ifndef ADS_PROJECT1_CURVESLSH_H
#define ADS_PROJECT1_CURVESLSH_H

#include <list>
#include <vector>
#include "util.h"
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
    typedef std::vector< std::vector<D> > CurveType;
    typedef std::vector< D > PointType;

    // Dimension of points - delta to be used ( delta =< 4*d*min{m1,m2} / m1,m2 : number of points in curves )
    // Number of grid
    int pointDim, delta, numGrids;
    // Parameters for LSH
    int k_vecs;
    // Parameters for Hypercube
    int k_hypercube, maxSearchPoints, probes;
    // Metric to be used
    std::string strMetric;
    // A uniformly random vector (1xd) and the resulting Grid Vector to snap points to
    std::vector<D> t, Gd;
    // List of 1d HashTables (LSH or Hypercube)
    std::list< CurveGridHT<D, Y, VH>* > GridHT_ListP;
    // Pointer of the function calculating distance between curves
    std::tuple<double, std::list<std::vector<int>>> (*fDist)(CurveType &a, CurveType &b, std::string metric_name );

public:

    CurvesLSH();

    CurvesLSH(int pointDim, int delta, int kVecs, int numGrids = 4, std::string metricName = "euclidean");

    CurvesLSH(int pointDim, int delta, int kHypercube, int maxSearchPoints, int probes, int numGrids = 4,
              std::string metricName = "euclidean");

    void addCurve ( CurveType &curve);
    void queryCurve ( CurveType &curve);

    virtual ~CurvesLSH() {

    }
};

// Constructor initializing LSH Vector Hash
template<class D, class Y, class VH>
CurvesLSH<D, Y, VH>::CurvesLSH(int pointDim, int delta, int kVecs, int numGrids, std::string metricName)
        :pointDim(pointDim), delta(delta), numGrids(numGrids), k_vecs(kVecs), strMetric(metricName) {

    // Create d number of gi hypercube hashTable functions
    for (int i = 0; i < numGrids; ++i) {
        //TODO: add arguments to LSH
        GridHT_ListP.push_back(new CurveGridHT<D, Y, VH>());
    }

    // Set the distance metric function to be used
    fDist = &dtw<CurveType, PointType, strMetric>;
}

// Constructor initializing Hypercube Vector Hash
template<class D, class Y, class VH>
CurvesLSH<D, Y, VH>::CurvesLSH(int pointDim, int delta, int kHypercube, int maxSearchPoints, int probes, int numGrids,
                               std::string metricName)
        :pointDim(pointDim), delta(delta), numGrids(numGrids), k_hypercube(kHypercube),
         maxSearchPoints(maxSearchPoints), probes(probes), strMetric(metricName) {

    // Create d number of gi hypercube hashTable functions

    for (int i = 0; i < numGrids; ++i) {
        //TODO: add arguments to hypercubes
        GridHT_ListP.push_back(new CurveGridHT<D, Y, VH>());
    }

    // Set the distance metric function to be used
    fDist = &dtw<CurveType, PointType, strMetric>;

}

// Function : Adding Curve to Grids
template<class D, class Y, class VH>
void CurvesLSH< D, Y, VH >::addCurve ( std::vector< std::vector<D> > &curve){

    typename std::list< CurveGridHT<D, Y, VH>* >::iterator gridHT;

    // Add curve to all Grid HTables
    for( gridHT = GridHT_ListP.begin(); gridHT != GridHT_ListP.end(); gridHT++ ) {
        // Add curve to current grid
        //TODO:: adding curve addition to grids functionality
    }

}

// Function : Querying for ANN Curve in Grids
template<class D, class Y, class VH>
void CurvesLSH< D, Y, VH >::queryCurve ( std::vector< std::vector<D> > &curve){

    typename std::list< CurveGridHT<D, Y, VH>* >::iterator gridHT;
    std::list<std::tuple<Y, D>> distanceList;

    // Search curve in all Grid HTables
    for( gridHT = GridHT_ListP.begin(); gridHT != GridHT_ListP.end(); gridHT++ ) {
        // Search curve in current grid
        //TODO:: adding curve querying in grids functionality
    }

}

#endif //ADS_PROJECT1_CURVESLSH_H
