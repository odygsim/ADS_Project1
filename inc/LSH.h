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
    int L, topLimit;                // L is the number of hash tables, radius the number of neighbors to return.
    double radius;
    // topLimit is the number of max points to search in bucket of each hash table.
    std::string metric_name = "manhattan", name = "LSH";  // the metric name to init the function that will calc distance.
    std::list<LSH_HT<TID, Y> *> htList;    // The list that will store the L hash tables.

    D (*f)(TID &, TID &); /* This is the function Pointer to selected metric its declaration is here
                     * and the definition an initialization*/

public:
    LSH(int d, int w = 6000, int k = 4, int L = 5, int m = 0, double radius = 0, int top_limit = 0,
        std::string metric_name = "manhattan"); // Constructor 1.

    ~LSH() { for (auto ht : htList) delete ht; }; // Destructor.

    std::string getName() const { return name; }

    void addX(TID &x, Y &y);  // Add point to structure.

    std::list<std::tuple<Y, D>> queryX(TID &x) const; // Query a Point it return a list of tuples (label, distance).
};


template<class TID, class D, class Y>
/*Usually TID: vector<int>, D: int, Y string*/
LSH<TID, D, Y>::LSH(int d, int w, int k, int L, int m, double radius, int top_limit, std::string metric_name):L(L),
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
        f = &manhattanDistance<D, TID>;
    else if (metric_name == "euclidean")     /* definition of the metric depending */
        f = &euclideanDistance<D, TID>;
    // check if topLimit has been given.
    if (topLimit == 0) topLimit = 3 * L;
    /* Create the k Hi functions in a list */
    for (int j = 0; j < L; ++j) {
        htList.push_back(new LSH_HT<TID, Y>(w, d, k, m, radius, topLimit));
    }
}


template<class TID, class D, class Y>
/*Usually TID: vector<int>, D: int, Y string*/
void LSH<TID, D, Y>::addX(TID &x, Y &y) {
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
std::list<std::tuple<Y, D>> LSH<TID, D, Y>::queryX(TID &x) const {
    /**
     * @brief Query a point.
     * @param x The object with data that the query will be executed.
     * @return A newly-constructed list, containing tuples<label,distance>.
     */

    typedef std::list<std::tuple<Y, D>> listTuples;                 // Declare Types list of tuples.
    typedef typename listTuples::iterator IteratorListTuples;     // Declare Iterator of list of tuples.

    listTuples distanceList,                                      // List that stores the distances from neighbors.
            labelDistanceList;                                 // List that stores the labels of neighbors.
    IteratorListTuples iterListTuples;                            // iterator to traverse the list.

    for (auto ht: htList) { // for all hastTables getPoints!
        for (auto point : ht->getPoint(x)) {
            // iterate through all returned points and append them in a list.
            distanceList.push_back({std::get<0>(point), f(std::get<1>(point), x)});
        }
    }

    distanceList.sort(TupleLess<1>()); // sort by neighbors
    IteratorListTuples itE = distanceList.end(); // iterator for traverse the list.
    // Now append the nearest neighbors
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

#endif //ADS_PROJECT1_LSH_H
