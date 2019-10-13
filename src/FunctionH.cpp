//
// Created by user on 10/11/19.
//

#include "../inc/FunctionH.h"
#include <random>
#include <iostream>

//FunctionH::FunctionH(unsigned int w = 0, unsigned int d = 0, unsigned int m = 0, unsigned int M = 0, unsigned int k = 4) {
FunctionH::FunctionH(unsigned int w, unsigned int d=0, unsigned int k=4){
    this->w = w;
    this->d = d;
    this->k = k;
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

int FunctionH::calculatePoint(const Point *p) {
    std::vector<int> data = p->getList();
    std::vector<int> A;
    int h = 0;

    // Calculate ai integers
    int maxA = INT32_MIN, di=0;

    if (d == 0) { //calculate it and produce S
        const int rangeFrom = 0, rangeTo = w;
        std::random_device randDev;
        std::mt19937 generator(randDev());
        std::uniform_real_distribution<double> distr(rangeFrom, rangeTo);

        d = p->getList().size();
        S.reserve(d);
        for (int i = 0; i < d; ++i) {
            S.push_back(distr(generator));
        }
    }
    A.reserve(d);
    for (int i = 0; i < d; ++i) {
        A.push_back((int) ((data[i] - S[i]) / w));
        if (A[i] > maxA) maxA = A[i];
    }
    if (m == 0) {
        m = maxA + 1; // m > maxAi
//        m = INT32_MAX - 5; // can use this!
    }
    if (M == 0) {
        unsigned int powerM = 32 / k;
        M = 1 << powerM;
    }
    std::vector<int>::iterator itS = A.begin();
    std::vector<int>::iterator itE = A.end();
    std::vector<int>::iterator it;
    for(di=0,it = itE;(di < d) && (it!=itS);--it, di++){
//        h += (*it) * (((int) pow(m, di)) % M) % M;
        h += (*it) * (int) pow(m, di) % M;
    }


    // TODO Calculate h


    return h;
}
