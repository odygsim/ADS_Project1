//
// Created by user on 10/11/19.
//

#ifndef ADS_PROJECT1_FUNCTIONH_H
#define ADS_PROJECT1_FUNCTIONH_H


#include <random>
//#include "Point.h"

template<class TID>
/* TID : usually vector<int> */
class FunctionH {
    unsigned int w, d, m, M, k;
    // w Is the window , d The dimension, m = 2^32 - 5, M 2^32/k, k used in M calculation
    std::vector<double> S;      // This is the vector of si's
    void initialize();

public:
    // Calculates h(x)
    int calculatePoint(const TID &x);

    FunctionH(unsigned int w, unsigned int d, unsigned int k);

    FunctionH(unsigned int w, unsigned int d, unsigned int k, int m);

    ~FunctionH() {}
//    FunctionH(unsigned int w, unsigned int d, unsigned int m=0, unsigned int M=0, unsigned int k=4);
//    int calculatePoint(const Point *);
};

template<typename TID>
void FunctionH<TID>::initialize() {

    unsigned int powerM = 32 / k;
    M = 1 << powerM;
//    this->M = 0;
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
}

template<class TID>
/* TID : usually vector<int> */
FunctionH<TID>::FunctionH(unsigned int w, unsigned int d, unsigned int k):w(w), d(d), k(k) {
    m = 0;
    initialize();
}

template<class TID>
/* TID : usually vector<int> */
FunctionH<TID>::FunctionH(unsigned int w, unsigned int d, unsigned int k, int m):w(w), d(d), k(k), m(m) {
    initialize();
}

template<class TID>
/* TID : usually vector<int> */
int FunctionH<TID>::calculatePoint(const TID &x) {
    /**
     * @brief Calculate the hi hash value of given object.
     * @param x The object with data that the hash function will be calculated.
     * @return int number the value of hash_function hi.
     */
    std::vector<int> A;
    int h = 0, maxA = INT32_MIN, di = 0;

    // Calculate ai integers
    A.reserve(d); //reserve size for ai
    double test;
    double testA;
    for (int i = 0; i < d; ++i) {
        test = x[i] - S[i];     // xi - si
        testA = (test / w);     // (xi - si) / w
        A.push_back(floor(testA)); // add the floor integer
//        A.push_back((int)((x[i] - S[i]) / w));
        if (A[i] > maxA) maxA = A[i];
    }
    if (m == 0) { m = maxA + 3; } // m > maxAi }
    // declare iterators
    typename TID::iterator itS = A.begin();
    typename TID::iterator itE = --A.end();
    typename TID::iterator iterAi;
    int t1, t2, t3, t4, mm = INT32_MAX - 5;
    for (di = 0, iterAi = itE; (di < d) && (iterAi != itS); --iterAi, di++) {
        t1 = pow(m, di);     // m^di
//        t1 = m*251;     // m*di
        if (m == mm)
            t1 = t1 % M;         // m^di % M
        t2 = (*iterAi);      // a d-1
        t3 = t2 * t1;        // a d-1 * (m^di %M)
        t4 = t3 % M;         // the result % M
        h += t4;

//        h += (*iterAi) * (((int) pow(m, di)) % M) % M;
//        h += (*iterAi) * (int) pow(m, di) % M;
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
