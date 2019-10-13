//
// Created by user on 10/11/19.
//

#ifndef ADS_PROJECT1_FUNCTIONH_H
#define ADS_PROJECT1_FUNCTIONH_H


#include <random>
#include "Point.h"

template <class TD, class D, class TY, class Y>
class FunctionH {
    unsigned int w,d,m,M, k;
    std::vector<double> S;

public:
    // Calculates h(x)
    int calculatePoint(const Point *);
    int calculatePoint(D x) ;
//    FunctionH(unsigned int w, unsigned int d) {
//
//    }

//    FunctionH(unsigned int w, unsigned int d, unsigned int m=0, unsigned int M=0, unsigned int k=4);
    FunctionH(unsigned int w, unsigned int d, unsigned int k);
    ~FunctionH(){}

};

template <class TD, class D, class TY, class Y>
int FunctionH<TD,D,TY,Y>::calculatePoint(const Point *p) {
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

//template<class TD, class D, class TY, class Y>
//int FunctionH<TD, D, TY, Y>::calculatePoint(D x) {
//    std::vector<int> data = x;
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
//    // TODO Calculate h
//
//
//    return h;
//    return 0;
//}


#endif //ADS_PROJECT1_FUNCTIONH_H
