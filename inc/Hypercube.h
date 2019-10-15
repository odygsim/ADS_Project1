//
// Created by aikio on 14/10/19.
//

#ifndef ADS_PROJECT1_HYPERCUBE_H
#define ADS_PROJECT1_HYPERCUBE_H

#include <list>
#include "Hypercube_HT.h"

template < class TD, class D, class TY, class Y>
class Hypercube {
private :
    // Parameters
    int d, k, m, M, probes, k_hi;
    double w, r;
    // Hypercube HashTables
    std::list< Hypercube_HT<TD, D, TY, Y> > HQ_HT_List;
    // Hypercube Hash Structure (vertices to points)
    std::unordered_multimap< int, const Point* > HQ_Buckets;

}
#endif //ADS_PROJECT1_HYPERCUBE_H
