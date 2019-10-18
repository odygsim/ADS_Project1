//
// Created by user on 10/11/19.
//

#ifndef ADS_PROJECT1_FUNCTIONH_H
#define ADS_PROJECT1_FUNCTIONH_H


#include <random>
#include "util.h"


template<class TID>
/* TID : usually vector<int> */
class FunctionH {
    unsigned int w, d=0, m, M, k;
    // w Is the window , d The dimension, m = 2^32 - 5, M 2^32/k, k used in M calculation
    std::vector<double> S;      // This is the vector of si's
    std::vector<unsigned int> Mi_mod_array; // Vector of (Mi^i) mod M results (i=0 to d-1)
    void initialize();

public:
    // Calculates h(x) old version
    int calculatePoint_old(const TID &x);
    // Calculates h(x)
    unsigned int calculatePoint(const TID &x);
    // Ai factors calculation
    std::vector<int> ai_calculation(TID &x);
    // Calculate hi(x) by the modulo multiplication of Ai, Mi terms
    unsigned int hi_calculation_by_Ai_Mi_mult(std::vector<int> &Ai,std::vector<unsigned int> &Mi, long M);
    // Calculate final h
    unsigned int h_calculation(TID x);

    FunctionH(unsigned int w, unsigned int d, unsigned int k);

    FunctionH(unsigned int w, unsigned int d, unsigned int k, int m);

    ~FunctionH() {}

};

template<typename TID>
void FunctionH<TID>::initialize() {

    unsigned int powerM = 32 / k;  // M = 2 ^ (32/k)
    M = 1 << powerM;
    const int rangeFrom = 0, rangeTo = w;
    // Create si's
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
    // If d has been given
    if (d>0) {
        // Calculate m (max ai) - assume a very large number m = 2^32 - 5
        unsigned int m = powl(2, 32) - 5;
        // Calculate in an array all modulo powers of m (i=0 to d-1)
        int res = calcModOfNumberInPower(m, d - 1, M, Mi_mod_array);
    }
}

template<class TID>
/* TID : usually vector<int> */
FunctionH<TID>::FunctionH(unsigned int w, unsigned int d, unsigned int k):w(w), d(d), k(k) {
    m = 0; // Constructor w/o m so init it with 0.
    initialize();
}

template<class TID>
/* TID : usually vector<int> */
FunctionH<TID>::FunctionH(unsigned int w, unsigned int d, unsigned int k, int m):w(w), d(d), k(k), m(m) {
    initialize();
}

template<class TID>
/* TID : usually vector<int> */
int FunctionH<TID>::calculatePoint_old(const TID &x) {
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
        if (A[i] > maxA) maxA = A[i];
    }
    if (m == 0) { m = maxA + 3; } // m > maxAi }
    // declare iterators
    typename TID::iterator itS = A.begin();
    typename TID::iterator itE = --A.end();
    typename TID::iterator iterAi;

    int t1, t2, t3, t4, mm = INT32_MAX - 5, t11, t12, t13, t14, h2 = 0;
    // Calculate sum (ai d1-1 * m^d0 +....+ ) % M , if m = big prime => lower it by modulo.
    for (di = 0, iterAi = itE; (di < d) && (iterAi != itS); --iterAi, di++) {
        // if m = big prime => lower it by modulo.
        if (m == mm){
            t1 = power(m, di, M);     // (m^di)% M
        }  // else m is max ai + 3
        else {
            t1 = pow(m, di);        // m^di
        }
        // calculate  ai * m^di
        t3 = (*iterAi) * t1;
        // add each calculated component to sum.
        h += t3;
    }
    h = power(h, 1, M);

    return h;
}

template<class TID>
/* TID : usually vector<int> */
unsigned int FunctionH<TID>::calculatePoint(const TID &x) {
    /**
     * @brief Calculate the hi hash value of given object.
     * @param x The object with data that the hash function will be calculated.
     * @return int number the value of hash_function hi.
     */

    unsigned int h;
    h = h_calculation(x);

    return h;
}

template<class TID>
unsigned int FunctionH<TID>::h_calculation(TID x){

    unsigned int h;
    std::vector<int> Ai;

    // Calculate Ai terms
    Ai = ai_calculation(x);
    // Calculate divided modulo calculation of h(x) calculation parts
    h = hi_calculation_by_Ai_Mi_mult(Ai, Mi_mod_array, this->M);

    return h;
}

template<class TID>
std::vector<int> FunctionH<TID>::ai_calculation(TID &x){
    double test;
    double testA;
    std::vector<int> A;

    for (int i = 0; i < d; ++i){
        // xi - si
        test = x[i] - S[i];
        // (xi-si)/w
        testA = (test / w);
        // Add the integer floor result
        A.push_back(floor(testA));
    }

    return A;
}

template<class TID>
unsigned int FunctionH<TID>::hi_calculation_by_Ai_Mi_mult(std::vector<int> &Ai,std::vector<unsigned int> &Mi, long M){

    std::vector<int>::const_reverse_iterator Ai_curr;
    std::vector<unsigned int>::const_iterator Mi_curr;

    unsigned  int res=0;
    // Perform term and factor wise calculations to avoid overflow
    for ( Ai_curr = Ai.crbegin(), Mi_curr = Mi.begin(); Ai_curr != Ai.crend() && Mi_curr != Mi.end(); ++Ai_curr, ++Mi_curr){
//        std::cout << "Ai = " << *Ai_curr << " Mi = " << *Mi_curr << "\n";
        res += trueMod ( trueMod(*Ai_curr, M) * trueMod(*Mi_curr, M), M);
    }
    // Final terms true modulo
    res = trueMod(res, M);

    return res;
}


#endif //ADS_PROJECT1_FUNCTIONH_H
