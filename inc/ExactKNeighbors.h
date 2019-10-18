//
// Created by ody on 17/10/19.
//

#ifndef ADS_PROJECT1_EXACTKNEIGHBORS_H
#define ADS_PROJECT1_EXACTKNEIGHBORS_H

#include "util.h"

/********************** ExactKNeighbors Part**********************************************************/
template<class TD, class TID, class D, class TY, class Y>
/*Usually TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
class ExactKNeighbors {
    /* This class is used as an algorithm for exact NN and has 2 methods addPoint, QueryPoint */
    int n_neighbors;            // Number of Neighbors to search.
    std::string metric_name;    // metric that will be used.
    TD data;                    // object that will store the data
    TY labels;                  // object that will store the labels

    D (*f)(TID &, TID &); /* This is the function Pointer to selected metric its declaration is here
                     * and the definition an initialization*/
public:
    ExactKNeighbors(int n_neighbors, std::string metric) : n_neighbors(n_neighbors), metric_name(metric) {
        /* Constructor and initialization */
        if (metric_name == "manhattan")     /* definition of the metric depending */
            f = &manhattanDistance<D, TID>;  /* on the metric_name argument passed to the constructor*/
    }

    void addPoint(TID &, Y &); // Add a Vector of int with its label
    std::list<std::tuple<Y, D>> queryPoint(TID &x); // Query a Point it return a list of tuples (label, distance)
};

template<class TD, class TID, class D, class TY, class Y>
/*Usually TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
void ExactKNeighbors<TD, TID, D, TY, Y>::addPoint(TID &x, Y &y) {
    /**
     * @brief Add key and value to according list.
     * @param x The first object with data.
     * @param y The second object with label.
     * @return void.
     */
    data.push_back(x);
    labels.push_back(y);
}

template<class TD, class TID, class D, class TY, class Y>
/*Usually TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
std::list<std::tuple<Y, D>> ExactKNeighbors<TD, TID, D, TY, Y>::queryPoint(TID &x) {
    /**
     * @brief Query a point.
     * @param x The object with data that the query will be executed.
     * @return A newly-constructed list, containing tuples<label,distance>.
     */

    typedef typename TD::iterator tdIt; // Iterator on the list of vectors
    typedef typename TY::iterator tyIt; // iterator on the list of strings
    tdIt iteratorData; // Init Iterator on list of vectors
    tdIt itDE = data.end(); // end of data iterator
    tyIt iteratorLabels; // Iterator on the list of strings



    typedef std::list<std::tuple<Y, D>> listTuples;
    typedef typename listTuples::iterator IteratorListTuples;

    listTuples distanceList;
    listTuples labelDistanceList;
    IteratorListTuples iterListTuples;
    int j;
    // Calculate distance from query point against all data.
    for (iteratorData = data.begin(), iteratorLabels = labels.begin();
         iteratorData != itDE; ++iteratorData, ++iteratorLabels) {
        distanceList.push_back(std::make_pair(*iteratorLabels, f(*iteratorData, (x)))); // Append distances to a list.
    }
    distanceList.sort(TupleLess<1>()); // sort distance list by neighbors
    IteratorListTuples itE = distanceList.end();
    // Now append the nearest neighbors to the return list
    for (j = 0, iterListTuples = distanceList.begin(); (j < this->n_neighbors) && (iterListTuples != itE); ++j, ++iterListTuples) {
        labelDistanceList.push_back(*iterListTuples);
    }

    return labelDistanceList;
}


#endif //ADS_PROJECT1_EXACTKNEIGHBORS_H