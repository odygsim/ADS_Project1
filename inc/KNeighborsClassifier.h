//
// Created by ody on 13/10/19.
//

#ifndef ADS_PROJECT1_KNEIGHBORSCLASSIFIER_H
#define ADS_PROJECT1_KNEIGHBORSCLASSIFIER_H

#include <string>
#include <list>
#include <utility>
#include <vector>
#include <iostream>
#include <chrono>
#include "util.h"


/********************** ExactKNeighbors Part**********************************************************/
template<class TD, class TID, class D, class TY, class Y>
/*Usually TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
class ExactKNeighbors {
    /* This class is used as an algorithm for exact NN and has 2 methods addPoint, QueryPoint */
    int n_neighbors;
    std::string metric_name;
    TD data;
    TY labels;

    D (*f)(TD, TD); /* This is the function Pointer to selected metric its declaration is here
                     * and the definition an initialization*/
public:
    ExactKNeighbors(int n_neighbors, std::string metric) : n_neighbors(n_neighbors),
                                                           metric_name(std::move(metric)) {
        /* Constructor and initialization */
        if (metric_name == "manhattan")     /* definition of the metric depending */
            f = &manhattanDistance<D, TD>;  /* on the metric_name argument passed to the constructor*/
    }
    void addPoint(TID ,Y); // Add a Vector of int with its label
    std::list<std::tuple<Y, D>> queryPoint(TID x) const; // Query a Point it return a list of tuples (label, distance)
};

template<class TD, class TID, class D, class TY, class Y>
/*Usually TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
void ExactKNeighbors<TD, TID, D, TY, Y>::addPoint(TID x, Y y) {
    /* Just push the key and value to the lists */
    data.push_back(x);
    labels.push_back(y);
}

template<class TD, class TID, class D, class TY, class Y>
/*Usually TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
std::list<std::tuple<Y, D>>
ExactKNeighbors<TD, TID, D, TY, Y>::queryPoint(TID x) const {
    // Query a Point it return a list of tuples (label, distance)
    typedef typename TD::iterator tdIt;
    typedef typename TY::iterator tyIt;
    typedef std::list<std::tuple<Y, D>> listTuples;
    typedef typename listTuples::iterator lTIt;

    listTuples distanceList;
    listTuples labelDistanceList;
    lTIt it;
    tdIt dIt;
    int j;

    for (unsigned long i = 0; i < data.size(); ++i)
        distanceList.push_back(std::make_pair(labels[i]->getName(),
                                              f(data[i]->getList(), (*dIt)->getList())));
//                                                      manhattanDistance<int, std::vector<int>>(data[i]->getList(), (*dIt)->getList())));
    distanceList.sort(TupleLess<2>());
    lTIt itS = distanceList.begin();
    lTIt itE = distanceList.end();
    for (j = 0, it = itS; (j < this->n_neighbors) && (it != itE); ++j, ++it) {
        labelDistanceList.push_back(*it);
    }

    return labelDistanceList;
}

/********************** KNeighbors Classifier Part**********************************************************/
/* A:lsh, cube,bruteForce TD: type of data list, vector  etc TY type of labels */
template<class A, class TD, class TID, class D, class TY, class Y>
/*Usually A:Algorithm to run class, TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
class KNeighborsClassifier {
    int n_neighbors;
    std::string algorithm_name;
    std::string metric_name;
    A *alg;
    TD data;
    TY labels;

    D (*f)(TID, TID);

public:
    KNeighborsClassifier(int n_neighbors, std::string algorithm, std::string metric) : n_neighbors(n_neighbors),
                                                                                       algorithm_name(
                                                                                               std::move(algorithm)),
                                                                                       metric_name(metric) {
        if (metric_name == "manhattan")
            f = &manhattanDistance<D, TID>;
//        if (algorithm == "bruteforce")
//            alg = this;
        std::cout << "KNN Initialization with " + algorithm_name << std::endl;
    }

    void fit(TD &x, TY &y); // Fit data of list<vector<int>>, and list< string>
    /* This method is used to get prediction not only for label but also for time and distance, specific for this
     * homework */
    std::list<std::tuple<double, std::list<Y, D>>> predictWithTimeAndDistance(TD x);
    /* This method is a test method for simple predict, just return list of labels*/
    std::list<std::list<Y>>
    predict(TD x); // return a list of lists that each list contains tuple(x.name, y.name), tuple(x.name, y.name)
};


template<class A, class TD, class TID, class D, class TY, class Y>
/*Usually A:Algorithm to run class, TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
void KNeighborsClassifier<A, TD, TID, D, TY, Y>::fit(TD &x, TY &y) {
/* General fit for all methods lsh, cube, exactKnn
 * Return: void
 * */
    data = x;
    labels = y;
    typedef typename std::vector<D>::iterator tdIt; // Iterator on the vector of data D
    typedef typename std::vector<Y>::iterator tyIt; // Iterator on the vector of labels Y
    tdIt e1 = data.end();
    tyIt e2 = labels.end();
    tdIt dIt;
    tyIt yIt;
    for (dIt = data.begin(), yIt = labels.begin(); (dIt != e1) && (yIt != e2); ++dIt, ++yIt) {
        alg->addPoint(*dIt, *yIt);
    }
}

template<class A, class TD, class TID, class D, class TY, class Y>
/*Usually A:Algorithm to run class, TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
std::list<std::tuple<double, std::list<Y, D>>>
KNeighborsClassifier<A, TD, TID, D, TY, Y>::predictWithTimeAndDistance(TD x) {
    /* This method will be used for the homework
     * Return: A list of tuples (timeValue, list(label, distance)) == list<tuple<double,list<string,double/int >>*/
    // Further explanation of return type
    /* Each record of tuple is (timeValue, list(tuple(label,distanceValue))
     * because each query has a time value and K neighbors, so will have a list of these tuples
     * and for all queries a list of of tuples(time, listOfNeighbors)
     * */
    typedef typename TD::iterator tdIt;
    typedef typename TY::iterator tyIt;
    typedef std::list<std::tuple<Y, D>> listTuples;
    typedef typename listTuples::iterator lTIt;

    listTuples distanceList;
    typedef std::list<std::tuple<double, std::list<Y, D>>> returnL;
    lTIt it;
    returnL returnList;
    int j;
    double elapsed = 0.0;

    tdIt e1 = x.end();
    tdIt dIt;
    for (dIt = x.begin(); dIt != e1; ++dIt) {
        listTuples labelDistanceList;
        // Start Time
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        labelDistanceList = alg->queryPoint(*dIt);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0;
        // End Time
        returnList.push_back(std::make_pair(elapsed, labelDistanceList));
    }

    return returnList;
}

/************ Test Code ************/

template<class A, class TD, class TID, class D, class TY, class Y>
/*Usually A:Algorithm to run class, TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
std::list<std::list<Y>> KNeighborsClassifier<A, TD, TID, D, TY, Y>::predict(TD x) {
    /* Classic Predict OUT OF SCOPE of this homework, return a list of labels */

    typedef typename TD::iterator tdIt;
    typedef typename TY::iterator tyIt;
    typedef std::list<std::tuple<Y, D>> listTuples;
    typedef typename listTuples::iterator lTIt;
    listTuples distanceList;
    typedef std::list<std::list<Y>> returnL;
    lTIt it;
    returnL returnList;
    int j;
    tdIt itD;
    tdIt itDE = data.end();
    tyIt itY;

    tdIt e1 = x.end();
    tdIt dItx;
    if (algorithm_name == "bruteforce") {

        for (dItx = x.begin(); dItx != e1; ++dItx) {
            std::list<Y> labelList;

            for (itD = data.begin(), itY = labels.begin(); itD != itDE; ++itD, ++itY)
                distanceList.push_back(std::make_pair(*itY, f(*itD, *dItx)));
//                                                      manhattanDistance<int, std::vector<int>>(data[i]->getList(), (*dItx)->getList())));
            distanceList.sort(TupleLess<1>());
            lTIt itS = distanceList.begin();
            lTIt itE = distanceList.end();
            for (j = 0, it = itS; (j < this->n_neighbors) && (it != itE); ++j, ++it) {
                labelList.push_back(std::get<0>(*it));
            }
            returnList.push_back(labelList);
        }
    }

    return returnList;
}

#endif //ADS_PROJECT1_KNEIGHBORSCLASSIFIER_H
