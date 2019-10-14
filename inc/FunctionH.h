//
// Created by user on 10/11/19.
//

#ifndef ADS_PROJECT1_FUNCTIONH_H
#define ADS_PROJECT1_FUNCTIONH_H


#include <random>
#include "Point.h"

template <class TID>
/* TID : usually vector<int> */
class FunctionH {
    unsigned int w,d,m,M, k;
    std::vector<double> S;

public:
    // Calculates h(x)
    int calculatePoint(const TID & x) ;
    FunctionH(unsigned int w, unsigned int d, unsigned int k);
    ~FunctionH(){}
//    FunctionH(unsigned int w, unsigned int d, unsigned int m=0, unsigned int M=0, unsigned int k=4);
//    int calculatePoint(const Point *);

};

template <class TID>
/* TID : usually vector<int> */
FunctionH<TID>::FunctionH(unsigned int w, unsigned int d, unsigned int k){
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

template<class TID>
/* TID : usually vector<int> */
int FunctionH<TID>::calculatePoint(const TID & x) {
    std::vector<int> A;
    int h = 0;

    // Calculate ai integers
    int maxA = INT32_MIN, di=0;
    m = INT32_MAX - 5; // can use this!

    if (d == 0) { //calculate it and produce S
        const int rangeFrom = 0, rangeTo = w;
        std::random_device randDev;
        std::mt19937 generator(randDev());
        std::uniform_real_distribution<double> distr(rangeFrom, rangeTo);

        d = x.size();
        S.reserve(d);
        for (int i = 0; i < d; ++i) {
            S.push_back(distr(generator));
        }
    }
    A.reserve(d); //reserve size for ai
    for (int i = 0; i < d; ++i) {
        A.push_back((int) ((x[i] - S[i]) / w));
//        if (A[i] > maxA) maxA = A[i];
    }
//    if (m == 0) {
//        m = maxA + 1; // m > maxAi
//    }
    if (M == 0) {
        unsigned int powerM = 32 / k;
        M = 1 << powerM;
    }
    // declare iterators
    typename TID::iterator itS = A.begin();
    typename TID::iterator itE = A.end();
    typename TID::iterator it;
    for(di=0,it = itE;(di < d) && (it!=itS);--it, di++){
//        h += (*it) * (((int) pow(m, di)) % M) % M;
        h += (*it) * (int) pow(m, di) % M;
    }
    return h;
}

//template <class TID>
//int FunctionH<TID>::calculatePoint(const Point *p) {
//    std::vector<int> data = p->getList();
//    std::vector<int> A;
//    int h = 0;
//
//    // Calculate ai integers
//    int maxA = INT32_MIN, di=0;
//
//    if (d == 0) { //calculate it and produce S
//        const int rangeFrom = 0, rangeTo = w;
//        std::random_device randDev;
//        std::mt19937 generator(randDev());
//        std::uniform_real_distribution<double> distr(rangeFrom, rangeTo);
//
//        d = p->getList().size();
//        S.reserve(d);
//        for (int i = 0; i < d; ++i) {
//            S.push_back(distr(generator));
//        }
//    }
//    A.reserve(d);
//    for (int i = 0; i < d; ++i) {
//        A.push_back((int) ((data[i] - S[i]) / w));
//        if (A[i] > maxA) maxA = A[i];
//    }
//    if (m == 0) {
//        m = maxA + 1; // m > maxAi
////        m = INT32_MAX - 5; // can use this!
//    }
//    if (M == 0) {
//        unsigned int powerM = 32 / k;
//        M = 1 << powerM;
//    }
//    std::vector<int>::iterator itS = A.begin();
//    std::vector<int>::iterator itE = A.end();
//    std::vector<int>::iterator it;
//    for(di=0,it = itE;(di < d) && (it!=itS);--it, di++){
////        h += (*it) * (((int) pow(m, di)) % M) % M;
//        h += (*it) * (int) pow(m, di) % M;
//    }
//
//
//
//
//    return h;
//}

#endif //ADS_PROJECT1_FUNCTIONH_H
