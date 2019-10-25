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
    /* This class is used as an algorithm for exact NN and has 2 methods addX, QueryX */
    double radius;            // Number of Neighbors to search.
    std::string metric_name, name = "ENN";    // metric that will be used.
    TD data;                    // object that will store the data
    TY labels;                  // object that will store the labels

    D (*f)(TID &, TID &, std::string); /* This is the function Pointer to selected metric its declaration is here */

public:
//    ExactKNeighbors(double radius, D (*f1) (TID &, TID &)):radius(radius), f1(f1) { }

    ExactKNeighbors(double radius, std::string metric) : radius(radius), metric_name(metric) {
        /* Constructor and initialization */
        if (metric_name == "manhattan" || metric_name == "euclidean")     /* definition of the metric depending */
            f = &lpNorm<D, TID>;
    }

    std::string getName() const { return name; }

    void addX(TID &, Y &); // Add a Vector of int with its label
    std::list<std::tuple<Y, D>> queryX(TID &x); // Query a Point it return a list of tuples (label, distance)
};

template<class TD, class TID, class D, class TY, class Y>
/*Usually TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
void ExactKNeighbors<TD, TID, D, TY, Y>::addX(TID &x, Y &y) {
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
std::list<std::tuple<Y, D>> ExactKNeighbors<TD, TID, D, TY, Y>::queryX(TID &x) {
    /**
     * @brief Query a point.
     * @param x The object with data that the query will be executed.
     * @return A newly-constructed list, containing tuples<label,distance>.
     */

    typedef typename TD::iterator tdIt; // Iterator on the list of vectors
    typedef typename TY::iterator tyIt; // iterator on the list of strings
    // Init Iterator on list of vectors, // end of data iterator
    tdIt iteratorData, itDE = data.end();
    tyIt iteratorLabels; // Iterator on the list of strings



    typedef std::list<std::tuple<Y, D>> listTuples;
    typedef typename listTuples::iterator IteratorListTuples;

    listTuples distanceList, labelDistanceList;
    IteratorListTuples iterListTuples, itE;
    int j;
    // Calculate distance from query point against all data.
    for (iteratorData = data.begin(), iteratorLabels = labels.begin();
         iteratorData != itDE; ++iteratorData, ++iteratorLabels) {
        distanceList.push_back(std::make_pair(*iteratorLabels, f(*iteratorData, x, metric_name))); // Append distances to a list.
    }
    distanceList.sort(TupleLess<1>()); // sort distance list by neighbors
    itE = distanceList.end();
    // Now append the nearest neighbors to the return list
    if ((radius) > 0) {
        for (iterListTuples = distanceList.begin();
             (iterListTuples != itE) && (radius < std::get<1>(*iterListTuples)); ++iterListTuples) {
            labelDistanceList.push_back(*iterListTuples);
        }
    } else {
        labelDistanceList.push_back(distanceList.front());
    }

    return labelDistanceList;
}

template<class TD, class TID, class D, class TY, class Y>
/*Usually TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
class ExactKNeighbors2 {
    /* This class is used as an algorithm for exact NN and has 2 methods addX, QueryX */
    double radius;            // Number of Neighbors to search.
    std::string metric_name, name = "ENN";    // metric that will be used.
    TD data;                    // object that will store the data
    TY labels;                  // object that will store the labels

    D (*f)(std::vector<std::vector<D>> &, std::vector<std::vector<D>> &); /* This is the function Pointer to selected metric its declaration is here */

public:
//    ExactKNeighbors(double radius, D (*f1) (TID &, TID &)):radius(radius), f1(f1) { }

    ExactKNeighbors2(double radius, std::string metric) : radius(radius), metric_name(metric) {
        /* Constructor and initialization */
        if (metric_name == "dtwEuclidean")     /* definition of the metric depending */{
            f = &dtw<std::vector<std::vector<double>>, TID, D>;
            metric_name = "euclidean";
        }
        else if (metric_name == "dtwManhattan")     /* definition of the metric depending */{
            f = &dtw<std::vector<std::vector<double>>, TID, D>;
            metric_name = "euclidean";
        }
    }

    std::string getName() const { return name; }

    void addX(TID &, Y &); // Add a Vector of int with its label
    std::list<std::tuple<Y, D>> queryX(TID &x); // Query a Point it return a list of tuples (label, distance)
};

template<class TD, class TID, class D, class TY, class Y>
/*Usually TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
void ExactKNeighbors2<TD, TID, D, TY, Y>::addX(TID &x, Y &y) {
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
std::list<std::tuple<Y, D>> ExactKNeighbors2<TD, TID, D, TY, Y>::queryX(TID &x) {
    /**
     * @brief Query a point.
     * @param x The object with data that the query will be executed.
     * @return A newly-constructed list, containing tuples<label,distance>.
     */

    typedef typename TD::iterator tdIt; // Iterator on the list of vectors
    typedef typename TY::iterator tyIt; // iterator on the list of strings
    // Init Iterator on list of vectors, // end of data iterator
    tdIt iteratorData, itDE = data.end();
    tyIt iteratorLabels; // Iterator on the list of strings



    typedef std::list<std::tuple<Y, D>> listTuples;
    typedef typename listTuples::iterator IteratorListTuples;

    listTuples distanceList, labelDistanceList;
    IteratorListTuples iterListTuples, itE;
    int j;
    // Calculate distance from query point against all data.
    for (iteratorData = data.begin(), iteratorLabels = labels.begin();
         iteratorData != itDE; ++iteratorData, ++iteratorLabels) {
        distanceList.push_back(std::make_pair(*iteratorLabels, f(*iteratorData, (x)))); // Append distances to a list.
    }
    distanceList.sort(TupleLess<1>()); // sort distance list by neighbors
    itE = distanceList.end();
    // Now append the nearest neighbors to the return list
    if ((radius) > 0) {
        for (iterListTuples = distanceList.begin();
             (iterListTuples != itE) && (radius < std::get<1>(*iterListTuples)); ++iterListTuples) {
            labelDistanceList.push_back(*iterListTuples);
        }
    } else {
        labelDistanceList.push_back(distanceList.front());
    }

    return labelDistanceList;
}

#endif //ADS_PROJECT1_EXACTKNEIGHBORS_H
