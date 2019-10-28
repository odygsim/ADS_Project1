//
// Created by aikio on 14/10/19.
//

#ifndef ADS_PROJECT1_HYPERCUBE_HT_H
#define ADS_PROJECT1_HYPERCUBE_HT_H

#include <list>
#include <unordered_map>
#include "FunctionH.h"
#include "util.h"

template<class TID>
class Hypercube_HT {

private:
    // Parameters
    int d, k, m, M, probes, k_hi;
    double w, r;
    // List of uniformly random hash functions h
    std::list< FunctionH<TID> * > hList;
    // Fi mapping : g to {0,1} uniformly
    std::unordered_map< unsigned int, int > fi_mapping;
    // Fi random pick
    int get_g_to_fi_mapping(unsigned int g);
    // G function calculation (by independent h functions)
    unsigned int calculate_g(const TID &p);
    // Uniform random generator for mapping g to {0,1} : f
    std::mt19937 fgenerator;

public:
    // Constructors-destructors
    Hypercube_HT(int d, double w = 3000, int k = 3, int m = INT32_MAX - 5, int M = 10, int probes = 2, int k_hi = 4, double r = 0);
    ~Hypercube_HT() { for (auto hf : hList) delete hf; };
    // Getting the fi for given point
    int get_fi(const TID &p) ;

};

// Hypercube HashTable constructor
template<class TID>
Hypercube_HT<TID>::Hypercube_HT(int d, double w, int k , int m, int M, int probes, int k_hi, double r) : fgenerator((std::random_device())())
{

    // Assign parameters
    this->w = w;
    this->d = d;
    this->k = k;
    this->m = m;
    this->M = M;
    this->probes = probes;
    this->k_hi = k_hi;
    this->r = r;

    // Get h new functions
    for ( int i=1 ; i <= k_hi ; i++) {
        hList.push_back(new FunctionH<TID> (w, d, k) );
    }

    // Create a uniform random generator for g to f {0,1} values
//    fgenerator(rndDevice);

}

//// Hypercube HashTable destructor
//template<class TID>
//Hypercube_HT<TID>::~Hypercube_HT(){
//
//    // Get hList start and end positions
//    typename std::list <FunctionH<TID> *>::iterator hListStart = hList.begin();
//    typename std::list <FunctionH<TID> *>::iterator hListEnd = hList.end();
//    // Erase all elements in hList
//    hList.erase(hListStart, hListEnd);
//
//}


// Get fi mapping for given g
template<class TID>
int Hypercube_HT<TID>::get_g_to_fi_mapping(unsigned int g){

    int fi = 0;
    // Get existing g->f mapping or create a new one
    if ( fi_mapping.find(g) == fi_mapping.end() ) {
        // create new (f, g) pair - add to mapping
        std::uniform_int_distribution<int> distr(0, 1);
        fi = distr(fgenerator);
        fi_mapping.insert( std::make_pair (g, fi) );
    } else {
        // existing (f, g) pair
        fi = fi_mapping.at(g);
    }

    return fi;
}

// Galculate g function based on given point p and hi's
template<class TID>
unsigned int Hypercube_HT<TID>::calculate_g(const TID &p){

    // Calculate g from the k Hi
    std::vector<unsigned int> hValues;
    unsigned int g;
    //Calculate the values of each h
    for(FunctionH<TID>* h: hList){
        hValues.push_back(h->calculatePoint(p));
    }
    // Get g from concat of hi's integer numbers bitwise
    g = (unsigned int) hValues[0];
    for (int i = 1; i < k_hi; ++i) {
        g = (g << k_hi) | (unsigned int) hValues[i]; //concatenate the k bits of all Hi
    }

    return g;
}

// Get final fi for given point p (only data)
template<class TID>
int Hypercube_HT<TID>::get_fi(const TID &p) {

    // Calculate g function for point p
    int g = calculate_g(p);
    // Get associate fi mapping to g
    int fi = get_g_to_fi_mapping(g);
    // return fi
    return fi;
}

#endif //ADS_PROJECT1_HYPERCUBE_HT_H
