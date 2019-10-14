//
// Created by aikio on 14/10/19.
//

#ifndef ADS_PROJECT1_HYPERCUBE_HT_H
#define ADS_PROJECT1_HYPERCUBE_HT_H

#include <list>
#include <unordered_map>
#include "FunctionH.h"
#include "util.h"

template < class TD, class D, class TY, class Y>
class Hypercube_HT {

private:
    // Parameters
    int d, k, m, M, probes, k_hi;
    // List of uniformly random hash functions h
    std::list< FunctionH<TD,D,TY,Y> * > hList;
    // Fi mapping : g to {0,1} uniformly
    std::unordered_map< int, int > fi_mapping;
    // Fi random pick
    int get_g_to_fi_mapping(int g);
    // G function calculation (by independent h functions)
    int calculate_g(const Point * p);

public:
    Hypercube_HT(int d, int k, int m, int M, int probes, int k_hi);
    ~Hypercube_HT();
    int get_fi(const Point *) ;
    int get_fi(D ,Y);

};

// Get fi mapping for given g
template < class TD, class D, class TY, class Y>
int Hypercube_HT<TD, D, TY, Y>::get_g_to_fi_mapping(int g){
    return 0;
}

// Galculate g function based on given point p and hi's
template < class TD, class D, class TY, class Y>
int Hypercube_HT<TD, D, TY, Y>::calculate_g(const Point *p){
    return 0;
}

// Get final fi for given point - case P is Point
template < class TD, class D, class TY, class Y>
int Hypercube_HT<TD, D, TY, Y>::get_fi(const Point *) {
    return 0;
}

// Get final fi for given point - case P is in "Data", "Label" format
template < class TD, class D, class TY, class Y>
int Hypercube_HT<TD, D, TY, Y>::get_fi(D data, Y label) {
    return 0;
}

#endif //ADS_PROJECT1_HYPERCUBE_HT_H
