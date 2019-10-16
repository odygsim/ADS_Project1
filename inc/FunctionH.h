//
// Created by user on 10/11/19.
//

#ifndef ADS_PROJECT1_FUNCTIONH_H
#define ADS_PROJECT1_FUNCTIONH_H


#include <random>
//#include "util.h"
//#include "Point.h"

int pow(int a, int b, int c);

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
        if (m == mm)
            t1 = pow(m, di, M);     // (m^di)% M
        else
            t1 = pow(m, di);        // m^di
        t3 = pow((*iterAi),1, M);        // a * 1 (%M)
        t4 = pow(t1*t3, 1, M);         //  (a*m) % M
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

/* For exponentiation, use the binary left-to-right algorithm
 * unless the exponent contains more than FIVEARY_CUTOFF digits.
 * In that case, do 5 bits at a time.  The potential drawback is that
 * a table of 2**5 intermediate results is computed.
 */
#define FIVEARY_CUTOFF 8

char * _digits_of_n(int n, int b) {
//""" Return the list of the digits in the base 'b'
//representation of n, from LSB to MSB
//"""
    std::vector<bool> digits;
    char *ar= new char[32];

    for(int i = 0; i < 32; i++){ //integer 32bits
        digits.push_back(n % b);
        n /= b;
    }
    return ar;
}

int pow(int a, int b, int c) {
    /**
     * @brief Calculates a^b % c, or else modular exponentiation.
     * @param a The number that will be the base.
     * @param b The number that will be the power.
     * @param c The number that will be the modulus.
     * @return a^b % c
     */
    // ideas from https://eli.thegreenplace.net/2009/03/28/efficient-modular-exponentiation-algorithms
    // http://cacr.uwaterloo.ca/hac/about/chap14.pdf
    // https://raw.githubusercontent.com/python/cpython/master/Objects/longobject.c
    int negativeOutput = 0;  /* if c<0 return negative output */
    long z = 1; /* accumulated result */
    long i, j, k, temp;/* counters */
    char digit = 0;
    k = 5;

    /* 5-ary values.  If the exponent is large enough, table is
     * precomputed so that table[i] == a**i % c for i in range(32).
     */
    long table[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    /* a, b, c = v, w, x */

//        if (b < 0 && c == NULL) {
//            /* if exponent is negative and there's no modulus:
//                   return a float.  This works because we know
//                   that this calls float_pow() which converts its
//                   arguments to double. */
//            return powf(a,b);
//        }
    if (c == 0) {
        perror("modulus is 0\n");
        exit(1);
    }

    /* if modulus < 0:
           negativeOutput = True
           modulus = -modulus */
    if (c < 0) {
        negativeOutput = 1;
        c = 0 - c;
    }

    /* if modulus == 1:
           return 0 */
    if (c == 1) return 0;

    /* if exponent is negative, negate the exponent and
       replace the base with a modular inverse */
    if (b < 0) {
        b = 0 - b;
        temp = a % c;
        a = temp;
    }

    /* Reduce base by modulus in some cases:
       1. If base < 0.  Forcing the base non-negative makes things easier.
       2. If base is obviously larger than the modulus.  The "small
          exponent" case later can multiply directly by base repeatedly,
          while the "large exponent" case multiplies directly by base 31
          times.  It can be unboundedly faster to multiply by
          base % modulus instead.
       We could _always_ do this reduction, but l_divmod() isn't cheap,
       so we only do it when it buys something. */
    if (a < 0 || a > c) {
        if ((temp = a % c) < 0){
//            perror("a%c < 0\n");
            temp = 0 - a;
//            exit(1);

        }
        a = temp;
    }

    /* At this point a, b, and c are guaranteed non-negative UNLESS
       c is NULL, in which case a may be negative. */

    /* Perform a modular reduction, X = X % c, but leave X alone if c
     * is NULL.
     */


    if (b <= FIVEARY_CUTOFF) {
        int base = 2;// << (5 -1);
        digit = 0;
        /* Left-to-right binary exponentiation (HAC Algorithm 14.79) */
        /* http://www.cacr.math.uwaterloo.ca/hac/about/chap14.pdf    */
        char *ar = _digits_of_n(b, base);
        for (i = 32 - 1; 0 <= i;i--) {
            digit = ar[i];
            z = z * z % c;
            if (digit == 1) { z = z * a % c; }
        }
        delete ar;
    } else {
        int base = 2 << (k - 1);

        /* Left-to-right 5-ary exponentiation (HAC Algorithm 14.82) */
        table[0] = z;
        // Precompute the table exponents
        for (i = 1; i < 32; ++i)
            table[i] = table[i - 1] * a % c;//table[i];
        char * ar = _digits_of_n(b, base);
        for (i = 32 - 1; 0 <= i;i--) {
            digit = ar[i];
            for (k = 0; k < 5; ++k)
                z = z * z % c;
            if (digit == 1)
                z = z * table[digit] % c;
        }
        delete ar;
    }

    if (negativeOutput && z != 0) {
        temp = z - c;
        z = temp;
    }
    return z;
}

#endif //ADS_PROJECT1_FUNCTIONH_H
