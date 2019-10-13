//
// Created by user on 10/11/19.
//

#include "../inc/FunctionH.h"
#include <random>
#include <iostream>

//FunctionH::FunctionH(unsigned int w = 0, unsigned int d = 0, unsigned int m = 0, unsigned int M = 0, unsigned int k = 4) {
template <class TD, class D, class TY, class Y>
FunctionH<TD,D,TY,Y>::FunctionH(unsigned int w, unsigned int d, unsigned int k){
    this->w = w;
    this->d = d;
    this->k = 4;
    this->m = 0;
    this->M = 0;
    const int rangeFrom = 0, rangeTo = w;
    // got random uniform idea from
    // https://stackoverflow.com/questions/288739/generate-random-numbers-uniformly-over-an-entire-range
    std::random_device randDev;
    std::mt19937 generator(randDev());
    std::uniform_real_distribution<double> distr(rangeFrom, rangeTo);
    if (d > 0) {
        S.reserve(d);
        for (int i = 0; i < d; ++i) {
            S.push_back(distr(generator));
        }
    }
    // check w should have one the following values
//    if (w == 0){
//        int r = 0;
//        w = 4*r;
//        // w = 10*r;
//    }
    //Print numbers
//    for (auto i: S) {
//        std::cout << S[i] << '\n';
//    }
}

