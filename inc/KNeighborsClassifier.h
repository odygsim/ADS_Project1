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


template<class A, class TD, class D, class TY, class Y>
class ExactKNeighbors {
    int n_neighbors;
    std::string metric_name;
    TD data;
    TY labels;

    D (*f)(TD, TD);

public:
    ExactKNeighbors(int n_neighbors, std::string metric) : n_neighbors(n_neighbors),
                                                           metric_name(std::move(metric)) {
        if (metric_name == "manhattan")
            f = &manhattanDistance<D, TD>;
    }

    void fit(TD &x, TY &y);

//    std::list<std::list<Y>>
//    predict(TD x); // return a list of lists that each list contains tuple(x.name, y.name), tuple(x.name, y.name)
//    std::list<std::tuple<double, std::list<Y, D>>> predictWithTimeAndDistance(TD x);

    std::list<std::tuple<Y, D>> queryPoint(TD x) const;
    // label name, time , distance

};

template<class A, class TD, class D, class TY, class Y>

void ExactKNeighbors<A, TD, D, TY, Y>::fit(TD &x, TY &y) {
    data = x;
    labels = y;
}

/* A:lsh, cube,bruteForce TD: type of data list, vector  etc TY type of labels */
template<class A, class TD, class TID, class D, class TY, class TIY, class Y>
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

    void fit(TD &x, TY &y);

    std::list<std::list<Y>>
    predict(TD x); // return a list of lists that each list contains tuple(x.name, y.name), tuple(x.name, y.name)
    std::list<std::tuple<double, std::list<Y, D>>> predictWithTimeAndDistance(TD x);

//    std::list<std::tuple<Y, D>> queryPoint(TD x) const;
    // label name, time , distance
};


template<class A, class TD, class TID, class D, class TY, class TIY, class Y>
void KNeighborsClassifier<A, TD, TID, D, TY, TIY, Y>::fit(TD &x, TY &y) {

    data = x;
    labels = y;
    if (algorithm_name == "bruteforce"){
        alg->fit(x, y);
        return;
    }
    else {
//        cout << 1 <<
//        typedef typename std::vector<D>::iterator tdIt;
//        typedef typename std::vector<Y>::iterator tyIt;
//        tdIt e1 = data.end();
//        tyIt e2 = labels.end();
//        tdIt dIt;
//        tyIt yIt;
//        for (dIt = data.begin(), yIt = labels.begin(); (dIt != e1) && (yIt != e2); ++dIt, ++yIt) {
//            alg->addPoint(*dIt, *yIt);
//        }
    }
}

template<class A, class TD, class TID, class D, class TY, class TIY, class Y>
std::list<std::tuple<double, std::list<Y, D>>>
KNeighborsClassifier<A, TD, TID, D, TY, TIY, Y>::predictWithTimeAndDistance(TD x) {
    //TODO
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

template<class A, class TD, class TID, class D, class TY, class TIY, class Y>
std::list<std::list<Y>> KNeighborsClassifier<A, TD, TID, D, TY, TIY, Y>::predict(TD x) {

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

template<class A, class TD, class D, class TY, class Y>
std::list<std::tuple<Y, D>>
ExactKNeighbors<A, TD, D, TY, Y>::queryPoint(TD x) const {
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

#endif //ADS_PROJECT1_KNEIGHBORSCLASSIFIER_H
