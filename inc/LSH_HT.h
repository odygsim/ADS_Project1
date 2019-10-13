//
// Created by ody on 13/10/19.
//

#ifndef ADS_PROJECT1_LSH_HT_H
#define ADS_PROJECT1_LSH_HT_H


#include <list>
#include <unordered_map>
#include "FunctionH.h"
#include "util.h"


template < class TD, class D, class TY, class Y>
class LSH_HT {
    int w, k, d, r;
    std::unordered_map<int, const Point *> ht;
    std::list<FunctionH<TD,D,TY,Y>* > hList;
    int calculateG(const Point * p) ;

public:
    LSH_HT(int w, int k, int, int);
//    LSH_HT<TD, D, TY, Y>::LSH_HT(int w, int d, int k = 4, int r=1000):w(w),k(k),d(d),r(r)
    ~LSH_HT();
    void fit();
    void addPoint(const Point *) ;
    void addPoint(D ,Y);
//    void LSH_HT<TD, D, TY, Y>::addPoint(D, Y);
    std::list<const Point *> getPoint(const Point *, int, int);
    std::list<const Point *> getPoint(TD, int, int);

};

template < class TD, class D, class TY, class Y>
class LSH {
    int L;
    std::list<LSH_HT<TD,TY, Y, D> *> htList;
public:
    LSH(int, int, int, int, int);
    LSH(int w, int d);
    void addPoint(const Point *) ;
    void addPoint(TD, TY);
    std::list<const qPoint *> queryPoint(const Point *, int radius, int) const;

//    template<class D, class Y>
    void addPoint(D x, Y y);
};


template < class TD, class D, class TY, class Y>
LSH_HT<TD, D, TY, Y>::LSH_HT(int w, int d, int k , int r):w(w),k(k),d(d),r(r){

    for (int j = 0; j < k; ++j) {
        hList.push_back(new FunctionH<TD,D,TY,Y> (4*r,d, k));
    }
}

template < class TD, class D, class TY, class Y>
LSH_HT<TD, D, TY, Y>::~LSH_HT(){
    typedef typename std::list <FunctionH<TD,D,TY,Y> *>::iterator functionHit;
    functionHit itS = hList.begin();
    functionHit itE = hList.end();
    functionHit it;
    for ( it = itS; it!=itE; ++it) {
        delete *it;
    }
}

template < class TD, class D, class TY, class Y>
int LSH_HT<TD, D, TY, Y>::calculateG(const Point * p) {
    /* Calculate the g from the k Hi */
    std::vector<int> hValues;
    int g = 0;
    //Calculate the values of each h
    for(auto h: hList){
        hValues.push_back(h->calculatePoint(p));
    }
    g = hValues[0];
    for (int i = 1; i < k; ++i) {
        g = (g << k) | hValues[i]; //concatenate the k bits of all Hi
    }
    return g;
}

template < class TD, class D, class TY, class Y>
void LSH_HT<TD, D, TY, Y>::addPoint(const Point * p) {/*Add Point to the hashtable */ ht[calculateG(p)] = p; }

template < class TD, class D, class TY, class Y>
void LSH_HT<TD, D, TY, Y>::addPoint(D x, Y y) {
    std::tuple<D, Y> result(x, y);
    /*Add Point to the hashtable */ ht[calculateG(x)] = result; }

template < class TD, class D, class TY, class Y>
std::list<const Point *> LSH_HT<TD, D, TY, Y>::getPoint(const Point * p, int radius, int topLimit) {
    /* Gather radius values from calculated g Key and return them in a list */
    int g = 0, i ;
    int limit = radius * topLimit; // multiply because if we want 5 neighbors we should get 5*topLimit
    std::list <const Point *> pList;
    typedef std::unordered_map<int, const Point *>::iterator umapIt;
    g = calculateG(p);
    // It returns a pair representing the range of elements with key equal to g
    std::pair<umapIt , umapIt > result = ht.equal_range(g);
    // Iterate over the range
    umapIt it;
    // Append to the list all the points between required range
    for(i = 0, it = result.first; (i< limit) && (it!=result.second) ; i++, it++){
        pList.push_back(it->second);
    }
    return pList;
}

//template < class TD, class D, class TY, class Y>
//void LSH_HT<TD, D, TY, Y>::addPoint(D , Y ) {

//}

template < class TD, class D, class TY, class Y>
std::list<const Point *> LSH_HT<TD, D, TY, Y>::getPoint(TD, int, int) {
    return std::list<const Point *>();
}

template < class TD, class D, class TY, class Y>
LSH<TD, D, TY, Y>::LSH(int w, int d, int k ,int L ,int r ):L(L) {

    /* Create the k Hi functions in a list */
    for (int j = 0; j < L; ++j) {
        htList.push_back(new LSH_HT<TD, D, TY, Y> (w, d, k, r));
    }
}

template < class TD, class D, class TY, class Y>
LSH<TD, D, TY, Y>::LSH(int w, int d) {
    int k = 4, r = 1000;
    L = 5;
    /* Create the k Hi functions in a list */
    for (int j = 0; j < L; ++j) {
        htList.push_back(new LSH_HT<TD, D, TY, Y> (w, d, k, r));
    }
}

template < class TD, class D, class TY, class Y>
void LSH<TD, D, TY, Y>::addPoint(const Point * p) {
    /* Add p to all L hastables */
    for( auto ht : htList){
        ht->addPoint(p);
    }
}

template < class TD, class D, class TY, class Y>
void LSH<TD, D, TY, Y>::addPoint(D x, Y y) {
    /* Add p to all L hastables */
    for( auto ht : htList){
        ht->addPoint(x, y);
    }
}

template < class TD, class D, class TY, class Y>
std::list<const qPoint *> LSH<TD, D, TY, Y>::queryPoint(const Point * p, int radius, int topLimit) const{
    std::list<const Point * > pList;
    typedef std::list<const qPoint *> qpList;
    typedef qpList::iterator qpIt;
    qpList queryPointList;
    qpList distanceList;
    qpList returnList;
    int j;

    for (auto ht: htList){
        for (auto point : ht->getPoint(p,radius, topLimit)){
            // iterate through all returned points and append them in a list.
            queryPointList.push_back( new qPoint(point->getName(), manhattanDistance<int,std::vector<int>>(point->getList(), p->getList())));
        }
    }

    distanceList.sort(ComparatorqPoint());

    qpIt itS = distanceList.begin();
    qpIt it;
    qpIt itE = distanceList.end();
    for (j = 0, it = itS; (j < radius) && (it!=itE); ++j, ++it) {
        returnList.push_back(*it);
    }
}
#endif //ADS_PROJECT1_LSH_HT_H
