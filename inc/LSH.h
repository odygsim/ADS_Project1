//
// Created by ody on 17/10/19.
//

#ifndef ADS_PROJECT1_LSH_H
#define ADS_PROJECT1_LSH_H

#include "LSH_HT.h"

/** LSH Part *
 *
 * @tparam TID  The object representing one data.
 * @tparam D    The type of data as ex. int/double.
 * @tparam Y    The type of label as ex. string.
 */
template<class TID, class D, class Y>
/*Usually TID: vector<int>, D: int, Y string*/
class LSH {
    int L, radius, topLimit;                // L is the number of hash tables, radius the number of neighbors to return.
    // topLimit is the number of max points to search in bucket of each hash table.
    std::string metric_name = "manhattan", name = "LSH";  // the metric name to init the function that will calc distance.
    std::list<LSH_HT<TID, Y> *> htList;    // The list that will store the L hash tables.

    D (*f)(TID &, TID &); /* This is the function Pointer to selected metric its declaration is here
                     * and the definition an initialization*/

public:
    LSH(int, int, int, int, int, int, int, std::string); // Constructor 1.

    LSH(int w, int d, int m, int radius, int topLimit, std::string); // Constructor 2.

    ~LSH() { for (auto ht : htList) delete ht; }; // Destructor.

    std::string getName() const {return name;}

    void addPoint(TID &x, Y &y);  // Add point to structure.

    std::list<std::tuple<Y, D>> queryPoint(TID &x) const; // Query a Point it return a list of tuples (label, distance).
};


template<class TID, class D, class Y>
/*Usually TID: vector<int>, D: int, Y string*/
LSH<TID, D, Y>::LSH(int w, int d, int k, int L, int m, int radius, int top_limit, std::string metric_name):L(L),
                                                                                                           radius(radius),
                                                                                                           topLimit(
                                                                                                                   top_limit),
                                                                                                           metric_name(
                                                                                                                   metric_name) {
    /**
     * @brief Constructor of this object.
     * @param w The Value of w.
     * @param d The dimension of the data.
     * @param k The number of hash functions to create.
     * @param L The number of hash tables to create.
     * @param m The number that will be used to calculate ai.
     * @param radius The number of points that a query will return.
     * @param top_limit The number of max points that a query will search to each hash table.
     * @param metric_name The name of the metric that will be used to calculate distance.
     * @return void.
     */

    if (metric_name == "manhattan")     /* definition of the metric depending */
        f = &manhattanDistance<D, TID>;  /* on the metric_name argument passed to the constructor*/
    /* Create the k Hi functions in a list */
    for (int j = 0; j < L; ++j) {
        htList.push_back(new LSH_HT<TID, Y>(w, d, k, m, radius, topLimit));
    }
}

template<class TID, class D, class Y>
/*Usually TID: vector<int>, D: int, Y string*/
LSH<TID, D, Y>::LSH(int w, int d, int m, int radius, int topLimit, std::string metric_name):radius(radius),
                                                                                            topLimit(topLimit),
                                                                                            metric_name(metric_name) {
    /**
     * @brief Constructor of this object.
     * @param w The Value of w.
     * @param d The dimension of the data.
     * @param radius The number of points that a query will return.
     * @param top_limit The number of max points that a query will search to each hash table.
     * @param metric_name The name of the metric that will be used to calculate distance.
     * @return void.
     */
    int k = 4;
    L = 5;
    if (metric_name == "manhattan")     /* definition of the metric depending */
        f = &manhattanDistance<D, TID>;  /* on the metric_name argument passed to the constructor*/
    /* Create the k Hi functions in a list */
    for (int j = 0; j < L; ++j) {
        htList.push_back(new LSH_HT<TID, Y>(w, d, k, m, radius, topLimit));
    }
}


template<class TID, class D, class Y>
/*Usually TID: vector<int>, D: int, Y string*/
void LSH<TID, D, Y>::addPoint(TID &x, Y &y) {
    /**
     * @brief Add key and value to all hash tables.
     * @param x The first object with data.
     * @param y The second object with label.
     * @return void.
     */
//    std::cout << "LSH: Adding Point " << y << std::endl ;
    for (auto ht : htList) {
        ht->addPoint(x, y);
    }
}

template<class TID, class D, class Y>
/*Usually TID: vector<int>, D: int, Y string*/
std::list<std::tuple<Y, D>> LSH<TID, D, Y>::queryPoint(TID &x) const {
    /**
     * @brief Query a point.
     * @param x The object with data that the query will be executed.
     * @return A newly-constructed list, containing tuples<label,distance>.
     */

    typedef std::list<std::tuple<Y, D>> listTuples;                 // Declare Types list of tuples.
    typedef typename listTuples::iterator IteratorListTuples;     // Declare Iterator of list of tuples.

    listTuples distanceList;                                      // List that stores the distances from neighbors.
    listTuples labelDistanceList;                                 // List that stores the labels of neighbors.
    IteratorListTuples iterListTuples;                            // iterator to traverse the list.
    int j;

    for (auto ht: htList) { // for all hastTables getPoints!
        for (auto point : ht->getPoint(x)) {
            // iterate through all returned points and append them in a list.
            distanceList.push_back(std::make_pair(std::get<0>(point), f(std::get<1>(point), x)));
        }
    }

    distanceList.sort(TupleLess<1>()); // sort by neighbors
    IteratorListTuples itE = distanceList.end(); // iterator for traverse the list.
    // Now append the nearest neighbors
    for (j = 0, iterListTuples = distanceList.begin();
         (j < this->radius) && (iterListTuples != itE); ++j, ++iterListTuples) {
        labelDistanceList.push_back(*iterListTuples);
    }

    return labelDistanceList;
}

#endif //ADS_PROJECT1_LSH_H
