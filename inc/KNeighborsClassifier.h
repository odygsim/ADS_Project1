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
//#include "util.h"
#include "LSH_HT.h"


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

    for (iteratorData = data.begin(), iteratorLabels = labels.begin();
         iteratorData != itDE; ++iteratorData, ++iteratorLabels) {
        distanceList.push_back(std::make_pair(*iteratorLabels, f(*iteratorData, (x))));
    }
//    for (unsigned long i = 0; i < data.size(); ++i)
//        distanceList.push_back(std::make_pair(labels[i], f(data[i], (x))));
    distanceList.sort(TupleLess<1>()); // sort by neighbors
    IteratorListTuples itS = distanceList.begin();
    IteratorListTuples itE = distanceList.end();
    // Now append the nearest neighbors
    for (j = 0, iterListTuples = itS; (j < this->n_neighbors) && (iterListTuples != itE); ++j, ++iterListTuples) {
        labelDistanceList.push_back(*iterListTuples);
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
    A alg;
    TD data;
    TY labels;

//    D (*f)(TID &, TID &);

public:
//    KNeighborsClassifier(int n_neighbors, std::string algorithm, std::string metric) : n_neighbors(n_neighbors),
//                                                                                       algorithm_name(
//                                                                                               std::move(algorithm)),
//                                                                                       metric_name(metric) {
///// OLD CONSTRUCTOR to be deleted?
//        using namespace ::std;
////        if (metric_name == "manhattan")
////            f = &manhattanDistance<D, TID>;
////        if (algorithm_name == "bruteforce")
////            alg = new ExactKNeighbors<list < vector<int>>, vector<int>, int, list < string >, string > (1, "manhattan");
////        else if (algorithm == "lsh")
////            alg = new LSH<TID, D, Y>(3000, 0, 1, 5, "manhattan");
////        else if (algorithm == "cube")
////            alg = new LSH<TID, D, Y>(3000, 0, 1, 5, "manhattan");
//        std::cout << "KNN Initialization with " + algorithm_name << std::endl;
//    }

    KNeighborsClassifier(int n_neighbors, A &alg) : n_neighbors(n_neighbors), alg(alg) {
        /**
         * @brief Constructor of this object.
         * @param n_neighbors The integer with the number of max neighbors to search.
         * @param alg The algorithm that will be used to classify data.
         * @return void.
         */
//        using namespace ::std;
//        if (metric_name == "manhattan")
//            f = &manhattanDistance<D, TID>;
//        if (algorithm == "bruteforce")
//            alg = new ExactKNeighbors<list < vector<int>>, vector<int>, int, list < string >, string > (1, "manhattan");
//        else if
//        std::cout << "KNN Initialization with " + algorithm_name << std::endl;
    }

    void fit(TD &x, TY &y); // Fit data of list<vector<int>>, and list< string>
    /* This method is used to get prediction not only for label but also for time and distance, specific for this
     * homework */
    std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> predictWithTimeAndDistance(TD &x);

    /* This method is a test method for simple predict, just return list of labels*/
//    std::list<std::list<Y>>
//    predict(TD &x); // return a list of lists that each list contains tuple(x.name, y.name), tuple(x.name, y.name)
};


template<class A, class TD, class TID, class D, class TY, class Y>
/*Usually A:Algorithm to run class, TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
void KNeighborsClassifier<A, TD, TID, D, TY, Y>::fit(TD &x, TY &y) {
    /**
     * @brief General fit for all methods lsh, cube, exactKnn.
     * @param x The first object with data.
     * @param y The second object with label.
     * @return void.
     */

    typedef typename TD::iterator tdIt; // Iterator on the list of vectors
    typedef typename TY::iterator tyIt; // iterator on the list of strings
    tdIt iteratorData; // Init Iterator on list of vectors
    tdIt itDE = data.end(); // end of data iterator
    tyIt iteratorLabels; // Iterator on the list of strings
    data = x;
    labels = y;

    for (iteratorData = data.begin(), iteratorLabels = labels.begin();
         iteratorData != itDE; ++iteratorData, ++iteratorLabels) {
        alg->addPoint(*iteratorData, *iteratorLabels); // Add a vector<int> and string
    }

}

template<class A, class TD, class TID, class D, class TY, class Y>
/*Usually A:Algorithm to run class, TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
std::list<std::tuple<double, std::list<std::tuple<Y, D>>>>
KNeighborsClassifier<A, TD, TID, D, TY, Y>::predictWithTimeAndDistance(TD &x) {
    /**
     * @brief General fit for all methods lsh, cube, exactKnn.
     * @param x The first object with data.
     * @return A newly-constructed list of tuples (timeValue, list(tuple<label, distance>))
     *
     * Each record of tuple is (timeValue, list(tuple(label,distanceValue))
     * because each query has a time value and K neighbors, so will have a list of these tuples
     * and for all queries a list of of tuples(time, listOfNeighbors)
     */
    typedef typename TD::iterator IteratorTD; // Iterator typedef on data
    typedef std::list<std::tuple<Y, D>> listTuples; // list of tuples <label,distances> , needed to calculcate neighbors
    typedef typename listTuples::iterator lTIt; // Iterator typedef on list of tuples

    listTuples distanceList; // distanceList to store all neighbors
    typedef std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> returnL; // typedef the return type because its big
    lTIt iteratorListTuples; // Iterator on list of tuples
    returnL returnList; // definition of return list
    double elapsed;

    IteratorTD e1 = x.end();
    IteratorTD iteratorData;
    for (iteratorData = x.begin(); iteratorData != e1; ++iteratorData) {
        listTuples labelDistanceList; // every query has new labelDistanceList
        // Start Time
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        labelDistanceList = alg->queryPoint(*iteratorData); // Query the point here send a vector<int>
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0;
        // End Time
        returnList.push_back(std::make_pair(elapsed, labelDistanceList));
    }

    return returnList;
}

/************ Test Code ************/

//template<class A, class TD, class TID, class D, class TY, class Y>
/*Usually A:Algorithm to run class, TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
//std::list<std::list<Y>> KNeighborsClassifier<A, TD, TID, D, TY, Y>::predict(TD &x) {
//    /* Classic Predict OUT OF SCOPE of this homework, return a list of labels */
//
//    typedef typename TD::iterator tdIt;
//    typedef typename TY::iterator tyIt;
//    typedef std::list<std::tuple<Y, D>> listTuples;
//    typedef typename listTuples::iterator lTIt;
//    listTuples distanceList;
//    typedef std::list<std::list<Y>> returnL;
//    lTIt it;
//    returnL returnList;
//    int j;
//    tdIt itD;
//    tdIt itDE = data.end();
//    tyIt itY;
//
//    tdIt e1 = x.end();
//    tdIt dItx;
//    if (algorithm_name == "bruteforce") {
//
//        for (dItx = x.begin(); dItx != e1; ++dItx) {
//            std::list<Y> labelList;
//
//            for (itD = data.begin(), itY = labels.begin(); itD != itDE; ++itD, ++itY)
//                distanceList.push_back(std::make_pair(*itY, f(*itD, *dItx)));
////                                                      manhattanDistance<int, std::vector<int>>(data[i]->getList(), (*dItx)->getList())));
//            distanceList.sort(TupleLess<1>());
//            lTIt itS = distanceList.begin();
//            lTIt itE = distanceList.end();
//            for (j = 0, it = itS; (j < this->n_neighbors) && (it != itE); ++j, ++it) {
//                labelList.push_back(std::get<0>(*it));
//            }
//            returnList.push_back(labelList);
//        }
//    }
//
//    return returnList;
//}

#endif //ADS_PROJECT1_KNEIGHBORSCLASSIFIER_H
