//
// Created by ody on 13/10/19.
//

#ifndef ADS_PROJECT1_LSH_HT_H
#define ADS_PROJECT1_LSH_HT_H


#include <list>
#include <unordered_map>
#include "FunctionH.h"
#include "util.h"

/**class LSH_HT * /
 *
 * @tparam TID The object that has the data.
 * @tparam Y  The object that has the labels.
 */
template<class TID, class Y>
/*Usually TID: vector<int>, Y string*/
class LSH_HT {
    int w, k, d, m, topLimit;
    double radius;
    std::unordered_multimap<unsigned int, std::tuple<Y &, TID &> > ht; // The type of hash table that will be created.
    std::list<FunctionH<TID> *> hList;               // The list that will store the L hash tables.

    unsigned int calculateG(TID &);

public:
    LSH_HT(int w, int d, int k, int m, double radius, int topLimit);

    ~LSH_HT();

    void addPoint(TID &, Y &); // Add a Vector of int with its label
    std::list<std::tuple<Y &, TID &>> getPoint(TID &);

};

/************* LSH_HT Methods Definitions *********************/
template<class TID, class Y>
/*Usually TID: vector<int>, Y string*/
LSH_HT<TID, Y>::LSH_HT(int w, int d, int k, int m, double radius, int topLimit):w(w), k(k), d(d), m(m), radius(radius),
                                                                             topLimit(topLimit) {

    /**
     * @brief Constructor of this object.
     * @param w The Value of w.
     * @param d The dimension of the data.
     * @param k The number of hash functions to create.
     * @param m The number that will be used to calculated ai.
     * @param radius The number of points that a query will return.
     * @param top_limit The number of max points that a query will search to each hash table.
     * @return void.
     */
    /* Constructor and initialize values of lsh_hashtable */
    for (int j = 0; j < k; ++j) {
        hList.push_back(new FunctionH<TID>(w, d, k, m));
    }
}

template<class TID, class Y>
/*Usually TID: vector<int>, Y string*/
LSH_HT<TID, Y>::~LSH_HT() {
    /* Destructor of lsh hashTable */
    typedef typename std::list<FunctionH<TID> *>::iterator functionHit;
    functionHit itS = hList.begin(), itE = hList.end(), it;
    for (it = itS; it != itE; ++it) {
        delete *it;
    }
}

template<class TID, class Y>
/*Usually TID: vector<int>, Y string*/
void LSH_HT<TID, Y>::addPoint(TID &x, Y &y) {
    /**
     * @brief Add key and value to according list.
     * @param x The first object with data.
     * @param y The second object with label.
     * @return void.
     */
    std::tuple<Y &, TID &> result(y, x);
    ht.insert({calculateG(x), result}); /*Add Point to the hashtable */
}

template<class TID, class Y>
/*Usually TID: vector<int>, Y string*/
std::list<std::tuple<Y &, TID & >> LSH_HT<TID, Y>::getPoint(TID &x) {
    /**
     * @brief Get a point from this ht.
     * @param x The object with data that the query will be executed.
     * @return A newly-constructed list, containing tuples<label,objectData>. objectData = (vector<int>)
     */

    std::list<std::tuple<Y &, TID & >> dataList;
    /* Gather radius values from calculated g Key and return them in a list */
    // Create a type of hashTable <int, tuple<label,vector<x>>
    typedef typename std::unordered_multimap<unsigned int, std::tuple<Y &, TID &>>::iterator umapIt;
    int g = 0, i;
    int limit = topLimit; // multiply because if we want 5 neighbors we should get 5*topLimit

    g = calculateG(x);
    // It returns a pair representing the range of elements with key equal to g
    std::pair<umapIt, umapIt> result = ht.equal_range(g);
    // Iterate over the range
    umapIt it;
    // Append to the list all the points between required range
    for (i = 0, it = result.first; (i < limit) && (it != result.second); i++, it++) {
        dataList.push_back(it->second);
    }
    return dataList;
}


template<class TID, class Y>
/*Usually TID: vector<int>, Y string*/
unsigned int LSH_HT<TID, Y>::calculateG(TID &x) {
    /**
     * @brief Calculate the g hash value of given object.
     * @param x The object with data that the hash function g will be calculated.
     * @return int number the value of hash_function g.
     */
    /* Calculate the g from the k Hi */
    std::vector<unsigned int> hValues;
    unsigned int g = 0;
    //Calculate the values of each h
    for (auto h: hList) {
        hValues.push_back(h->calculatePoint(x));
    }
    g = hValues[0];
    for (int i = 1; i < k; ++i) {
        g = (g << k) | hValues[i]; //concatenate the k bits of all Hi
    }
    return g;
}

#endif //ADS_PROJECT1_LSH_HT_H
