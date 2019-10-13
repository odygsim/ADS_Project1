//
// Created by user on 10/11/19.
//

#ifndef ADS_PROJECT1_FUNCTIONH_H
#define ADS_PROJECT1_FUNCTIONH_H


#include "Point.h"

class FunctionH {
    unsigned int w,d,m,M, k;
    std::vector<double> S;

public:
    // Calculates h(x)
    int calculatePoint(const Point *);
//    FunctionH(unsigned int w, unsigned int d) {
//
//    }

//    FunctionH(unsigned int w, unsigned int d, unsigned int m=0, unsigned int M=0, unsigned int k=4);
    FunctionH(unsigned int w, unsigned int d, unsigned int k);
    ~FunctionH(){}

};


#endif //ADS_PROJECT1_FUNCTIONH_H
