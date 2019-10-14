//
// Created by ody on 13/10/19.
//

#ifndef ADS_PROJECT1_LSH_HT_H
#define ADS_PROJECT1_LSH_HT_H


#include <list>
#include <unordered_map>
#include "FunctionH.h"
#include "util.h"

template<class TID, class Y>
/*Usually TID: vector<int>, Y string*/
class LSH_HT {
    int w, k, d, r, radius, topLimit;
//    std::unordered_map<int, const Point *> ht;
    std::unordered_map<int, std::tuple<Y, TID> > ht;
    std::list<FunctionH<TID> *> hList;

    int calculateG(TID &);

//    int calculateG(const Point * p) ;

public:
    LSH_HT(int w, int d, int, int, int, int);

//    LSH_HT<TD, D, TY, Y>::LSH_HT(int w, int d, int k = 4, int r=1000):w(w),k(k),d(d),r(r)
    ~LSH_HT();

    void addPoint(TID &, Y &); // Add a Vector of int with its label
    std::list<std::tuple<Y, TID>> getPoint(TID &);

//    void addPoint(const Point *) ;
//    void LSH_HT<TD, D, TY, Y>::addPoint(D, Y);
//    std::list<const Point *> getPoint(const Point *, int, int);
    /* Return a list of tuples <string, vector<int>> */

};

/************* LSH_HT Methods Definitions *********************/
template<class TID, class Y>
/*Usually TID: vector<int>, Y string*/
LSH_HT<TID, Y>::LSH_HT(int w, int d, int k, int r, int radius, int topLimit):w(w), k(k), d(d), r(r), radius(radius),
                                                                             topLimit(topLimit) {
    /* Constructor and initialize values of lsh_hashtable */
    for (int j = 0; j < k; ++j) {
        hList.push_back(new FunctionH<TID>(4 * r, d, k));
    }
}

template<class TID, class Y>
/*Usually TID: vector<int>, Y string*/
LSH_HT<TID, Y>::~LSH_HT() {
    /* Destructor of lsh hashTable */
    typedef typename std::list<FunctionH<TID> *>::iterator functionHit;
    functionHit itS = hList.begin();
    functionHit itE = hList.end();
    functionHit it;
    for (it = itS; it != itE; ++it) {
        delete *it;
    }
}

template<class TID, class Y>
/*Usually TID: vector<int>, Y string*/
void LSH_HT<TID, Y>::addPoint(TID &x, Y &y) {
    std::tuple<Y, TID> result(y, x);
    /*Add Point to the hashtable */ ht[calculateG(x)] = result;
}

template<class TID, class Y>
/*Usually TID: vector<int>, Y string*/
std::list<std::tuple<Y, TID>> LSH_HT<TID, Y>::getPoint(TID &x) {
    /* Return: a list of tuples <string, vector<int>> */

    std::list<std::tuple<Y, TID>> dataList;
    /* Gather radius values from calculated g Key and return them in a list */
    // Create a type of hashTable <int, tuple<label,vector<x>>
    typedef typename std::unordered_map<int, std::tuple<Y, TID>>::iterator umapIt;
    int g = 0, i;
    int limit = radius * topLimit; // multiply because if we want 5 neighbors we should get 5*topLimit

    g = calculateG(x);
    // It returns a pair representing the range of elements with key equal to g
    std::pair<umapIt, umapIt> result = ht.equal_range(g);
    // Iterate over the range
    umapIt it;
    // Append to the list all the points between required range
    for (i = 0, it = result.first; (i < limit) && (it != result.second); i++, it++) {
        // TODO check this
        dataList.push_back(it->second);
    }
    return dataList;
}


template<class TID, class Y>
/*Usually TID: vector<int>, Y string*/
int LSH_HT<TID, Y>::calculateG(TID &x) {
    /* Calculate the g from the k Hi */
    std::vector<int> hValues;
    int g = 0;
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


/********************** LSH Part *************************************/
template<class TID, class D, class Y>
/*Usually TID: vector<int>, D: int, Y string*/
class LSH {
    int L, radius, topLimit;
    std::string metric_name = "manhattan";
    std::list<LSH_HT<TID, Y> *> htList;

    D (*f)(TID &, TID &); /* This is the function Pointer to selected metric its declaration is here
                     * and the definition an initialization*/

public:
    LSH(int, int, int, int, int, int, int, std::string);

    LSH(int w, int d, int radius, int topLimit, std::string);

//    void addPoint(const Point *) ;
//    std::list<const qPoint *> queryPoint(const Point *, int radius, int) const;
//    std::list<const qPoint *> queryPoint(const Point *, int radius, int) const;
    void addPoint(TID &x, Y &y);

    std::list<std::tuple<Y, D>> queryPoint(TID &x) const; // Query a Point it return a list of tuples (label, distance)
};


template<class TID, class D, class Y>
/*Usually TID: vector<int>, D: int, Y string*/
LSH<TID, D, Y>::LSH(int w, int d, int k, int L, int r, int radius, int top_limit, std::string metric_name):L(L),
                                                                                                           radius(radius),
                                                                                                           topLimit(
                                                                                                                   top_limit),
                                                                                                           metric_name(
                                                                                                                   metric_name) {

//    if (metric_name == "manhattan")     /* definition of the metric depending */
        f = &manhattanDistance<D, TID>;  /* on the metric_name argument passed to the constructor*/
    /* Create the k Hi functions in a list */
    for (int j = 0; j < L; ++j) {
        htList.push_back(new LSH_HT<TID, Y>(w, d, k, r));
    }
}

template<class TID, class D, class Y>
/*Usually TID: vector<int>, D: int, Y string*/
LSH<TID, D, Y>::LSH(int w, int d, int radius, int topLimit, std::string metric_name):radius(radius), topLimit(topLimit),
                                                                                     metric_name(metric_name) {
    int k = 4, r = 1000;
    L = 5;
    /* Create the k Hi functions in a list */
    for (int j = 0; j < L; ++j) {
        htList.push_back(new LSH_HT<TID, Y>(w, d, k, r, radius, topLimit));
    }
}


template<class TID, class D, class Y>
/*Usually TID: vector<int>, D: int, Y string*/
void LSH<TID, D, Y>::addPoint(TID &x, Y &y) {
    /* Add p to all L hastables */
    for (auto ht : htList) {
        ht->addPoint(x, y);
    }
}

template<class TID, class D, class Y>
/*Usually TID: vector<int>, D: int, Y string*/
std::list<std::tuple<Y, D>> LSH<TID, D, Y>::queryPoint(TID &x) const {
    /* Query a Point iterListTuples
    * Return: a list of tuples (label, distance) */

    typedef std::list<std::tuple<Y, D>> listTuples;
    typedef typename listTuples::iterator IteratorListTuples;

    listTuples distanceList;
    listTuples labelDistanceList;
    IteratorListTuples iterListTuples;
    int j;

    for (auto ht: htList) { // for all hastTables getPoints!
        for (auto point : ht->getPoint(x)) {
            // iterate through all returned points and append them in a list.
            j = manhattanDistance<int, std::vector<int>>(std::get<1>(point), x);
            distanceList.push_back(std::make_pair(std::get<0>(point), j));
                    //f(std::get<1>(point), x)));
        }
    }

    distanceList.sort(TupleLess<1>()); // sort by neighbors
    IteratorListTuples itE = distanceList.end();
    // Now append the nearest neighbors
    for (j = 0, iterListTuples = distanceList.begin();
         (j < this->radius) && (iterListTuples != itE); ++j, ++iterListTuples) {
        labelDistanceList.push_back(*iterListTuples);
    }

    return labelDistanceList;
}

//template < class TD, class D, class TY, class Y>
///*Usually TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
//void LSH<TD, D, TY, Y>::addPoint(const Point * p) {
//    /* Add p to all L hastables */
//    for( auto ht : htList){
//        ht->addPoint(p);
//    }
//}
//template < class TD, class D, class TY, class Y>
///*Usually TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
//std::list<const qPoint *> LSH<TD, D, TY, Y>::queryPoint(const Point * p, int radius, int topLimit) const{
//    std::list<const Point * > pList;
//    typedef std::list<const qPoint *> qpList;
//    typedef qpList::iterator qpIt;
//    qpList queryPointList;
//    qpList distanceList;
//    qpList returnList;
//    int j;
//
//    for (auto ht: htList){
//        for (auto point : ht->getPoint(p,radius, topLimit)){
//            // iterate through all returned points and append them in a list.
//            queryPointList.push_back( new qPoint(point->getName(), manhattanDistance<int,std::vector<int>>(point->getList(), p->getList())));
//        }
//    }
//
//    distanceList.sort(ComparatorqPoint());
//
//    qpIt itS = distanceList.begin();
//    qpIt it;
//    qpIt itE = distanceList.end();
//    for (j = 0, it = itS; (j < radius) && (it!=itE); ++j, ++it) {
//        returnList.push_back(*it);
//    }
//}
/*************************************** Old methos of lsh_ht point *****************************/

/*********** methods on LSH_HT Point **************/
//template < class TD, class D, class TY, class Y>
//int LSH_HT<TD, D, TY, Y>::calculateG(const Point * p) {
//    /* Calculate the g from the k Hi */
//    std::vector<int> hValues;
//    int g = 0;
//    //Calculate the values of each h
//    for(auto h: hList){
//        hValues.push_back(h->calculatePoint(p));
//    }
//    g = hValues[0];
//    for (int i = 1; i < k; ++i) {
//        g = (g << k) | hValues[i]; //concatenate the k bits of all Hi
//    }
//    return g;
//}

//template < class TD, class D, class TY, class Y>
//void LSH_HT<TD, D, TY, Y>::addPoint(const Point * p) {/*Add Point to the hashtable */ ht[calculateG(p)] = p; }

//template < class TD, class D, class TY, class Y>
//std::list<const Point *> LSH_HT<TD, D, TY, Y>::getPoint(const Point * p, int radius, int topLimit) {
//    /* Gather radius values from calculated g Key and return them in a list */
//    int g = 0, i ;
//    int limit = radius * topLimit; // multiply because if we want 5 neighbors we should get 5*topLimit
//    std::list <const Point *> pList;
//    typedef std::unordered_map<int, const Point *>::iterator umapIt;
//    g = calculateG(p);
//    // It returns a pair representing the range of elements with key equal to g
//    std::pair<umapIt , umapIt > result = ht.equal_range(g);
//    // Iterate over the range
//    umapIt it;
//    // Append to the list all the points between required range
//    for(i = 0, it = result.first; (i< limit) && (it!=result.second) ; i++, it++){
//        pList.push_back(it->second);
//    }
//    return pList;
//}

//template < class TD, class D, class TY, class Y>
//void LSH_HT<TD, D, TY, Y>::addPoint(D , Y ) {

//}
#endif //ADS_PROJECT1_LSH_HT_H
