//
// Created by aikio on 22/10/19.
//

#ifndef ADS_PROJECT1_CURVEGRIDHT_H
#define ADS_PROJECT1_CURVEGRIDHT_H

#include <vector>
#include <random>
#include <cmath>
//#include <tuple>
//#include <string>
#include "util.h"
//#include "util2.h"
#include "LSH.h"
#include "Hypercube.h"

template<class D, class Y, class VH>
class CurveGridHT {

private:

    // Setting types for Curves and Points to be easily used throughout class
    typedef std::vector<std::vector<D> > CurveType;
    typedef std::vector<std::vector<double> > GridCurveType;
    typedef std::vector<D> PointType;
    typedef std::vector<double> GridPointType;
    typedef std::vector<double> ConcatVector;
    typedef std::tuple<CurveType, Y> curveDataType;

    // Setting type for LSH, Hypercube
    typedef LSH< std::vector<double>, double, std::tuple< CurveType, Y > > LSHType;
    typedef Hypercube< std::vector<double>, double, std::tuple< CurveType, Y > > HQType;

    // Dimension of points - delta to be used ( delta =< 4*d*min{m1,m2} / m1,m2 : number of points in curves )
    // Maximum number of points in a curve to apply padding for hashing 1d vectors - assume 10% more than max points of curves in dataset
    int pointDim, maxPointsForPadding;
    double delta, maxCoord;
    // Name for the metric to be used internally in fdist
    std::string metric_name;
    // Parameters for LSH
    int k_vecs;
    // Parameters for Hypercube
    int k_hypercube, maxSearchPoints, probes;
    // Parameters for both
    double w;
    // A uniformly random vector (1xd) and the resulting Grid Vector to snap points to
    std::vector<D> t, Gd;
    // Grid Point containing zeros for padding purposes
    GridPointType zeroGridPoint;
    // Hashing of resulting vector in one dimension
    VH* oneDimHashing;
    // Uniform random generator for mapping g to {0,1} : f
    std::mt19937 fgenerator;

    // Pointer of the function calculating distance between curves
    std::tuple<double, std::list<std::vector<int>>> (*fDist)(CurveType &a, CurveType &b, std::string metric_name);

    // Initialize grid
    void initializeGrid();

    // Create a Grid Point of PointDim dimension containing zeros to pad grid curves with
    void initZeroGridPoint(double maxCoord );

    // Calculate curve's corresponding grid curve
    GridCurveType calcGridCurve(CurveType &curve);

    // Get point snapped to grid
    GridPointType snappedToGrid(PointType &p);

    // Concat grid points to single vector including padding
    ConcatVector concatGridPointsToVector(GridCurveType &gCurve);


public:
    CurveGridHT() {}

    // Constructor for LSH one dim hashing
    CurveGridHT(int pointDim, double delta, int maxPointsForPadding, int kVecs, double w = 3000,
                std::string metric_name = "euclidean", double maxCoord = 50);

    // Constructor for Hypercube one dim hashing
    CurveGridHT(int pointDim, double delta, int maxPointsForPadding, int kHypercube, int maxSearchPoints, int probes,
                double w = 3000, std::string metric_name = "euclidean", double maxCoord = 50);

    // Destructor for LSH/Hypercube structures
    virtual ~CurveGridHT() { delete oneDimHashing ; };

    // For LSH assume L=1 and k (num of hi(x)) could be given
    // For Hypercube all arguments k, M, probes are given - or default

    // Add curve to CurvesLSH
    void addCurve(CurveType &curve, Y &y);

    // Query a curve in Curves LSH
//    std::tuple<Y, CurveType >
    std::list<std::tuple<Y, D>> queryCurve(CurveType &curve);
};

template<class D, class Y, class VH>
std::vector< std::vector<double> > CurveGridHT<D, Y, VH>::calcGridCurve(CurveType &curve) {

    // Resulting grid curve
    GridCurveType gridCurve;
    // Types of grid points (current/previous in grid curve)
    GridPointType currGridPoint, prevGridPoint;
    // Current curve point
    typename CurveType::iterator currPoint;

    // Get the first point in curve
    currPoint = curve.begin();
    // Set it as the first previous grid point
    prevGridPoint = snappedToGrid(*currPoint);
    // Add it to grid curve
    gridCurve.push_back(prevGridPoint);
    // Increment to new curve point
    currPoint++;

    // Loop over all points and snap them to grid (if current different to previous grid point)
    for ( currPoint; currPoint != curve.end(); currPoint++ ){
        // Snap current point to curve
        currGridPoint = snappedToGrid((*currPoint) );
        // Check if it is the same with previous
        if ( currGridPoint != prevGridPoint ) {
            gridCurve.push_back(currGridPoint);
        }
        // Set current as previous
        prevGridPoint = currGridPoint;
    }

    // Padding for keeping same points
    int diffToPad = maxPointsForPadding -  gridCurve.size();

    for ( int i=0; i < diffToPad ; i++) {
        gridCurve.push_back(zeroGridPoint);
    }

    return gridCurve;
}

template<class D, class Y, class VH>
std::vector<double> CurveGridHT<D, Y, VH>::snappedToGrid(PointType &p) {

    // Coordinate types for point and grid point
    typename PointType::iterator pCoord;
    typename GridPointType::iterator gCoord;
    // Integer index value of respective grid i-th coordinate
    int gCoordIndex;
    // Final grid i-th coordinate value
    double newCoord;
    // Respective snapped grid point
    CurveGridHT::PointType snappedPoint;

    // Loop over i-th coordinates to find respective i-th grid coordinates
    for (pCoord = p.begin(), gCoord = Gd.begin(); pCoord != p.end(), gCoord != Gd.end(); pCoord++, gCoord++) {
        // Divide to found nearest integer point in i-th dimension
        gCoordIndex = std::lround(*pCoord / *gCoord);
        // Get point coordinates in i-th dimension
        newCoord = gCoordIndex * (*gCoord);
        // Add snapped i-th grid coordinate to grid point
        snappedPoint.push_back(newCoord);
    }

    return snappedPoint;
}

template<class D, class Y, class VH>
std::vector<double> CurveGridHT<D, Y, VH>::concatGridPointsToVector(GridCurveType &gCurve) {

    // Grid point for : a) concatenation of grid points, b) as temp grid point of curve
    ConcatVector gridConcatVector;

    // Concatenate grid curve points to a vector
    GridCurveType::iterator gPoint;
    for ( gPoint = gCurve.begin() ; gPoint != gCurve.end(); gPoint++){
        gridConcatVector.insert(gridConcatVector.end(), gPoint->begin(), gPoint->end() );
    }

    return gridConcatVector;

}

template<class D, class Y, class VH>
void CurveGridHT<D, Y, VH>::addCurve(CurveType &curve, Y &y) {

    // Grid curve
    GridCurveType gCurve;
    // Grid point for : a) concatenation of grid points, b) as temp grid point of curve
    GridPointType gridConcatVector;

    // Get respective grid curve
    gCurve = calcGridCurve( curve );

    // Concatenate grid curve points to a vector
    gridConcatVector = concatGridPointsToVector(gCurve);

    // Make a tuple of (pointer to curve , curve label)
    curveDataType curveData = std::make_tuple(curve, y);

    // Hash vector to one dim hash structure
    oneDimHashing->addX(gridConcatVector, curveData);

}

template<class D, class Y, class VH>
std::list<std::tuple<Y, D>> CurveGridHT<D, Y, VH>::queryCurve(CurveType &curve){

    // Type definition of query result
    typedef std::list< std::tuple< std::tuple<CurveType, Y>, double > >  neighbList;
//    // Type definition of result p
//    typedef std::tuple< std::tuple<CurveType, Y>, double >* resPointer;

    // Query results of one dim hashing
    neighbList oneDimQueryResults;
    // Iterator of neighbors in one dim hash results
    typename neighbList::const_iterator currNeigh;
    // Current and Best Neighbor data holders
    curveDataType currCurveData;
    CurveType currCurve;
    Y currCurveLabel ="";
    Y bestCurveLabel ="";
    double bestDist = DBL_MAX;
    double currDist = 0;

    // Grid curve
    GridCurveType gCurve;

    // Grid point for : a) concatenation of grid points, b) as temp grid point of curve
    GridPointType gridConcatVector;

    // Get respective grid curve
    gCurve = calcGridCurve( curve );

    // Concatenate grid curve points to a vector
    gridConcatVector = concatGridPointsToVector(gCurve);

    //Get neighbors stored in one dim Hash Table
    oneDimQueryResults = oneDimHashing->queryX( gridConcatVector );
    int i=0;
    // Find the minimum distance neighbor
    for ( currNeigh = oneDimQueryResults.begin(); currNeigh != oneDimQueryResults.end() ; ++currNeigh ) {
        i++;
//        std::cout << "currNeigh = " << i << '\n';
        // Getting current neighbor curve data tuple : (Curve, label)
        currCurveData = std::get<0>(*currNeigh);
        // Getting curve from curve data tuple
        currCurve = std::get<0>(currCurveData);
        // Calculating distance between query and current curve
        currDist = std::get<0>(fDist(curve, currCurve, metric_name));

        if ( currDist < bestDist ) {
            // Set new best distance
            bestDist = currDist;
            // Get and set new best curve data label
            currCurveLabel = std::get<1>(currCurveData);
            bestCurveLabel = currCurveLabel;
        }
    }

    // Hold (label,distance) best neighbour tuple
    std::list<std::tuple<Y, D>> distanceList;
    // Check if no neighbour found
    if ( bestCurveLabel != "" ){
        distanceList.push_back(std::make_tuple(bestCurveLabel, bestDist));
    }

    return distanceList;

}

template<class D, class Y, class VH>
void CurveGridHT<D, Y, VH>::initializeGrid() {

    // Vectors t and Gd coordinates
    double ti, gdi;

    // Uniform random generator for mapping g to {0,1} : f
    std::uniform_real_distribution<double> distr(0, delta);

    // Create Grid representing vector Gd
    for (int i = 0; i < pointDim; i++) {
        // Generate random ti coordinate to add to delta
        ti = distr(fgenerator);
        // Generate Gd = p + t (vectors : p represents the grid vector created by delta and t the random vector (0,d]^d )
        gdi = ti + delta;
        // Add to final grid vector
        Gd.push_back(gdi);
    }

}

template<class D, class Y, class VH>
void CurveGridHT<D, Y, VH>::initZeroGridPoint(double maxCoord) {

    double padNum = 100 * maxCoord;
    // Create grid point of pointDim dimension full of great values
    for (int i=0 ; i < pointDim ; i++) {
        zeroGridPoint.push_back(padNum);
    }

}

template<class D, class Y, class VH>
CurveGridHT<D, Y, VH>::CurveGridHT(int pointDim, double delta, int maxPointsForPadding, int kVecs, double w, std::string metric_name, double maxCoord) :
                        pointDim(pointDim), delta(delta), maxPointsForPadding(maxPointsForPadding), k_vecs(kVecs), w(w),
                        metric_name(metric_name), maxCoord(maxCoord),
                        fgenerator((std::random_device())()){

    // Set the distance metric function to be used
    fDist = &dtw<CurveType, PointType, D>;

    // Initialize grid and zero grid point for padding
    this->initializeGrid();
    this->initZeroGridPoint(maxCoord);

    // Create a new LSH hash structure for one dimensional hashing of grid curve
    oneDimHashing = new LSHType(pointDim, w , k_vecs, 1, 0, DBL_MAX, 0, "manhattan");

}

template<class D, class Y, class VH>
CurveGridHT<D, Y, VH>::CurveGridHT(int pointDim, double delta, int maxPointsForPadding, int kHypercube,
                                   int maxSearchPoints, int probes,
                                   double w, std::string metric_name, double maxCoord):
    pointDim(pointDim), delta(delta), maxPointsForPadding(maxPointsForPadding), k_hypercube(kHypercube),
    maxSearchPoints(maxSearchPoints), probes(probes), w(w), metric_name(metric_name), maxCoord(maxCoord),
    fgenerator((std::random_device())()){

    // Set the distance metric function to be used
    fDist = &dtw<CurveType, PointType, D>;

    // Initialize grid and zero grid point for padding
    this->initializeGrid();
    this->initZeroGridPoint(maxCoord);

    // Create a new Hypercube hash structure for one dimensional hashing of grid curve
    oneDimHashing = new HQType(pointDim, w, k_hypercube, maxSearchPoints, probes, 4, DBL_MAX);

}

#endif //ADS_PROJECT1_CURVEGRIDHT_H
