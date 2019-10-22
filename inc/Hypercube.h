//
// Created by aikio on 14/10/19.
//

#ifndef ADS_PROJECT1_HYPERCUBE_H
#define ADS_PROJECT1_HYPERCUBE_H

#include <string>
#include <list>
#include <unordered_map>
#include <algorithm>
#include "util.h"
#include "util2.h"
#include "Hypercube_HT.h"

template<class TID, class D, class Y>
class Hypercube {

private :
    // Parameters
    int d, k, maxSearchPoints, probes, k_hi;
    int m;
    double w, r;
    // Hypercube HashTables
    std::list< Hypercube_HT<TID> * > HQ_HT_List;
    // Hypercube Hash Structure (vertices to points)
    std::unordered_multimap< int, std::tuple<Y, TID> > HQ_Buckets;
    // Function reference for calculating distance according to specific metric
    D (*f)(TID &, TID &);
    // Calculate a point's corresponding hypercube vertice and return to integer representation
    int calculate_point_corresponding_vertice(const TID &x);
    // Get the near vertices (only 1 bit change)
    std::list<int> get_vertice_candidate_adjacent_buckets(int v);

public:

    Hypercube(int d, double w = 3000, int k = 3, int maxSearchPoints = 10, int probes = 2, int k_hi = 4, double r = 0);
    ~Hypercube(){};
    // Adding a point with it's label to Hypercube Hash structure
    void addPoint(TID &x, Y &y);
    // Query a point : return a list of ANN in (label,distance) tuples
    std::list<std::tuple<Y, D>> queryPoint(TID &x);

};

template<class TID, class D, class Y>
Hypercube<TID, D, Y>::Hypercube(int d, double w, int k, int maxSearchPoints, int probes, int k_hi, double r):
        d(d), w(w), k(k), maxSearchPoints(maxSearchPoints), probes(probes), k_hi(k_hi), r(r){

    // Set default parameters
    m = INT32_MAX - 5;
    k_hi = 4;

    // Create d number of gi hypercube hashTable functions
    for (int i = 0; i < k; ++i) {
        HQ_HT_List.push_back(new Hypercube_HT<TID>(d, w, k, m, maxSearchPoints, probes, k_hi, r));
    }

    // Set the distance metric function to be used
    f = &manhattanDistance<D, TID>;
}

template<class TID, class D, class Y>
void Hypercube<TID, D, Y>::addPoint(TID &x, Y &y) {

    // Find corresponding hypercube vertice
    int v = calculate_point_corresponding_vertice(x);
    // Create a (data,label) tuple to add to bucket
    std::tuple<Y, TID> result(y, x);
    // Add point to Hypercube
    HQ_Buckets.insert( {v, result} );

}

template<class TID, class D, class Y>
std::list<std::tuple<Y, D>> Hypercube<TID, D, Y>::queryPoint(TID &x) {

    // Hold current vertice and distance during search
    int currVert;
    D currDist;
    unsigned  int j;
    // Hold current number of search points
    int currSP = 0;
    // Hold (label,distance) neighbour tuples
    std::list<std::tuple<Y, D>> distanceList;
    std::list<std::tuple<Y, D>> distanceLabelList;

    // Get corresponding hypercube vertice
    int v = calculate_point_corresponding_vertice(x);

    // Get adjacent canditate vertices including main one
    std::list<int> candVert = get_vertice_candidate_adjacent_buckets(v);

    // Get candidate vertices list size
    int candVertSz = candVert.size();

    // Create iterator for Hypercube Buckets
    typename std::unordered_multimap< int, std::tuple<Y, TID> >::iterator mapBucket;

    // Loop until reaching max probes given or no-more candidate vertices to search
    for (int i=1 ; (i <= probes+1) && ( i <= candVertSz) ; i++) {
        // Get and remove current candidate vertice
        currVert = candVert.front();
        candVert.pop_front();
        // Get corresponding vertice in Hypercube if it exists
        mapBucket = HQ_Buckets.find(currVert);
        // Check in current Bucket (max : up to MaxSearchPoints number)
        if ( mapBucket != HQ_Buckets.end() ) {
            // Get hypercube bucket number where currVert is in
            j = HQ_Buckets.bucket(currVert);

            // Check all points within the bucket
            for (auto local_it = HQ_Buckets.begin(j); local_it != HQ_Buckets.end(j); ++local_it) {
                //
                if (++currSP > maxSearchPoints) {
                    break;
                }
                // Get neighbor point data and label
                TID currNeighbor = std::get<1>(local_it->second);
                Y currNeighLabel = std::get<0>(local_it->second);
                // Get the distance of given point from current neighbor point
                currDist = f(currNeighbor, x);
                // Add (label, distance) tuple to distance list
                distanceList.push_back(std::make_pair(currNeighLabel, currDist));
            }

        }
    }

//    // Create a vector in order to sort Neighbors tuples
//    std::vector< std::tuple<Y,D> > neighResVector;
//    typename std::list< std::tuple<Y, D>>::iterator tupleIter;
//    typename std::vector< std::tuple<Y, D>>::iterator vecIter;
//
//    // Fill vector with distanceList tuple data
//    for( tupleIter = distanceList.begin() ; tupleIter != distanceList.end(); tupleIter++ ){
//        neighResVector.push_back(*tupleIter);
//    }
//    // Sort vector according to the distance element of tuples
//    std::sort(begin(neighResVector), end(neighResVector), TupleLess<1>());
//
//    // Create new tupples distance list
//    for( vecIter = neighResVector.begin() ; vecIter != neighResVector.end(); vecIter++ ){
//        distanceLabelList.push_back(*vecIter);
//    }

    // Sort list according to the distance element of tuples (neighbor label, distance)
    distanceList.sort(TupleLess<1>());

    typename std::list< std::tuple<Y, D>>::iterator tupleIter;
    // Radius or best neighbor results
    if ( r > 0 ) {
        // Get neighbors within radius r
        for (tupleIter = distanceList.begin(); tupleIter != distanceList.end(); tupleIter++) {
            // Get distance of current neighbor
            currDist = std::get<1>(*tupleIter);
            // Add neighbor if its distance is less than r
            if ( currDist <= r ) {
                distanceLabelList.push_back(*tupleIter);
            }
        }

    } else {
        // Return nearest neighbor
        distanceLabelList.push_back( distanceList.front() );
    }

    // Return neighbors
    return distanceLabelList;
}

template<class TID, class D, class Y>
int Hypercube<TID, D, Y>::calculate_point_corresponding_vertice(const TID &x) {

    int fi;
    int ftotal;
    std::string res_str_fi;
    typename std::list< Hypercube_HT<TID>* >::iterator HT_Iter;

    // Iterate through all Hypercube HashTables to get fi's
    for (HT_Iter = HQ_HT_List.begin(); HT_Iter != HQ_HT_List.end(); ++HT_Iter) {
        fi = (*HT_Iter)->get_fi(x);
        res_str_fi += std::to_string(fi);
    }

    // Convert concatenated fi's bit string to integer
    ftotal = std::stoi(res_str_fi, nullptr, 2);

    return ftotal;
}

template<class TID, class D, class Y>
std::list<int> Hypercube<TID, D, Y>::get_vertice_candidate_adjacent_buckets(int v){

    std::list<int> resList;
    // Add main vertice to search
    resList.push_back(v);

    int i, bitValue, nearV;

    // Alter all bits of vertice to get possible candidate vertices
    for (i = 1; i <= k; i++) {
        // get bit in i-th position
        bitValue = get_int_bit_value(v, i);
        // change it and set it to get the new integer value
        if (bitValue == 0) { bitValue = 1; } else { bitValue = 0; }
        nearV = set_int_bit_value(v, i, bitValue);
        // add it to candidate list
        resList.push_back(nearV);
    }

    return resList;

}

#endif //ADS_PROJECT1_HYPERCUBE_H
