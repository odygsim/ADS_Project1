//
// Created by ody on 13/10/19.
//

#ifndef ADS_PROJECT1_KNEIGHBORSCLASSIFIER_H
#define ADS_PROJECT1_KNEIGHBORSCLASSIFIER_H

#include "LSH.h"
#include "ExactKNeighbors.h"


/********************** KNeighbors Classifier Part**********************************************************/
/* A:lsh, cube,bruteForce TD: type of data list, vector  etc TY type of labels */
template<class A, class TD, class TID, class D, class TY, class Y>
/*Usually A:Algorithm to run class, TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
class KNeighborsClassifier {
    int n_neighbors;
    A alg;
    TD data;
    TY labels;

public:

    KNeighborsClassifier(int n_neighbors, A &alg) : n_neighbors(n_neighbors), alg(alg) {
        /**
         * @brief Constructor of this object.
         * @param n_neighbors The integer with the number of max neighbors to search.
         * @param alg The algorithm that will be used to classify data.
         * @return void.
         */
    }

    void fit(TD &x, TY &y); // Fit data of list<vector<int>>, and list< string>
    /* This method is used to get prediction not only for label but also for time and distance, specific for this
     * homework */
    std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> predictWithTimeAndDistance(TD &x);
    /* This method is a test method for simple predict, just return list of labels*/
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
    std:: cout << data.size() << std::endl;
    // fit all data to alg algorithm's structure.
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
     *
     */
    typedef typename TD::iterator IteratorTD; // Iterator typedef on data
    typedef std::list<std::tuple<Y, D>> listTuples; // list of tuples <label,distances> , needed to calculcate neighbors
    typedef typename listTuples::iterator lTIt; // Iterator typedef on list of tuples
    typedef std::chrono::steady_clock::time_point timePoint;
    typedef std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> returnL; // typedef the return type because its big

    timePoint start;
    double elapsed;
    listTuples distanceList; // distanceList to store all neighbors
    lTIt iteratorListTuples; // Iterator on list of tuples
    returnL returnList; // definition of return list

    IteratorTD e1 = x.end();
    IteratorTD iteratorData;
    for (iteratorData = x.begin(); iteratorData != e1; ++iteratorData) {
        listTuples labelDistanceList; // every query has new labelDistanceList
        start = initTime(); // Start Time
        labelDistanceList = alg->queryPoint(*iteratorData); // Query the point here send a vector<int>
        elapsed = getElapsed(start); // End Time
        returnList.push_back(std::make_pair(elapsed, labelDistanceList)); // Append to the return list the time and the distance list return by alg query method
    }

    return returnList;
}


#endif //ADS_PROJECT1_KNEIGHBORSCLASSIFIER_H
