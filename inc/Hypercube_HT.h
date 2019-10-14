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
    double w, r;
    // List of uniformly random hash functions h
    std::list< FunctionH<TD,D,TY,Y> * > hList;
    // Fi mapping : g to {0,1} uniformly
    std::unordered_map< int, int > fi_mapping;
    // Fi random pick
    int get_g_to_fi_mapping(int g);
    // G function calculation (by independent h functions)
    int calculate_g(const Point * p);
    // Uniform random generator for mapping g to {0,1} : f
    std::mt19937 fgenerator;
    std::uniform_int_distribution<int> f_distr;

public:
    Hypercube_HT(double w, int d, int k, int m, int M, int probes, int k_hi, double r);
    ~Hypercube_HT();
    // Getting the fi for given point - point class type
    int get_fi(const Point *) ;
    // Getting the fi for given point - data/label type
    int get_fi(D ,Y);

};

// Hypercube HashTable constructor
template < class TD, class D, class TY, class Y>
Hypercube_HT<TD, D, TY, Y>::Hypercube_HT(double w = 4, int d, int k = 3, int m = 2^32-5, int M, int probes = 2, int k_hi = 4, double r){

    // Assign parameters
    this->d = d;
    this->k = k;
    this->m = m;
    this->M = M;
    this->probes = probes;
    this->k_hi = k_hi;
    this->r = r;

    // Get h new functions
    for ( int i=1 ; i <= k_hi ; i++) {
        hList.push_back(new FunctionH<TD, D, TY, Y> (4*r, d, k) );
    }

    // Create a uniform random generator for g to f {0,1} values
    std::random_device rndDev;
    std::mt19937 generator(rndDev());
    std::uniform_int_distribution<int> udistr(0, 1);

    // Set generator and distribution to the class level
    this->fgenerator = generator;
    this->f_distr() = udistr;

}

// Hypercube HashTable destructor
template < class TD, class D, class TY, class Y>
Hypercube_HT<TD, D, TY, Y>::~Hypercube_HT(){

    // Get hList start and end positions
    typename std::list <FunctionH<TD,D,TY,Y> *>::iterator hListStart = hList.begin();
    typename std::list <FunctionH<TD,D,TY,Y> *>::iterator hListEnd = hList.end();
    // Erase all elements in hList
    hList.erase(hListStart, hListEnd);

}


// Get fi mapping for given g
template < class TD, class D, class TY, class Y>
int Hypercube_HT<TD, D, TY, Y>::get_g_to_fi_mapping(int g){

    // Get existing g->f mapping or create a new one
    if ( fi_mapping.find(g) == fi_mapping.end() ) {
        // create new (f, g) pair
        int newFmapping = f_distr(fgenerator);
        fi_mapping.insert( std::make_pair (g, newFmapping) );
        return newFmapping;
    } else {
        // existing (f, g) pair
        int fmapping = fi_mapping.at(g);
        return fmapping;
    }
}

// Galculate g function based on given point p and hi's
template < class TD, class D, class TY, class Y>
int Hypercube_HT<TD, D, TY, Y>::calculate_g(const Point *p){

    // Calculate the g from the k Hi
    std::vector<int> hValues;
    int g = 0;
    //Calculate the values of each h
    for(auto h: hList){
        hValues.push_back(h->calculatePoint(p));
    }
    // Get g from concat of hi's integer numbers
    std::string gstr;
    for (int i = 0; i < k; ++i) {
        gstr += std::to_string(hValues[i]); //concatenate all hi' s
    }

    // Convert concatenated string to integer
    int g = std::stoi(gstr);
    return g;
}

// Get final fi for given point - case P is Point
template < class TD, class D, class TY, class Y>
int Hypercube_HT<TD, D, TY, Y>::get_fi(const Point * p) {
    int g;

    // Calculate g function for point p
    int g = calculate_g(p);
    // Get associate fi mapping to g
    int fi = get_g_to_fi_mapping();
    // return fi
    return fi;
}

// Get final fi for given point - case P is in "Data", "Label" format
template < class TD, class D, class TY, class Y>
int Hypercube_HT<TD, D, TY, Y>::get_fi(D data, Y label) {

    // Todo : code to be completed for point data provided in this format
    return 0;
}

#endif //ADS_PROJECT1_HYPERCUBE_HT_H
