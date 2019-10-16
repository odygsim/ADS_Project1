//
// Created by user on 10/11/19.
//

#ifndef ADS_PROJECT1_FUNCTIONH_H
#define ADS_PROJECT1_FUNCTIONH_H


#include <random>
#define FIVEARY_CUTOFF 8
#define ll long long

#include <bits/stdc++.h>
using namespace std;
#define lint long long int
#define S second

//* Iterative Function to calculate (x^y)%p in O(log y) */
int power(int x, unsigned int y, int p)
{
    int res = 1;      // Initialize result

    x = x % p;  // Update x if it is more than or
    // equal to p

    while (y > 0)
    {
        // If y is odd, multiply x with result
        if (y & 1)
            res = (res*x) % p;

        // y must be even now
        y = y>>1; // y = y/2
        x = (x*x) % p;
    }
    return res;
}

lint modex(lint base,lint exponent,lint mod){
    if(base == 0){                                  // 0^x = 0.
        return 0;
    }
    if(exponent == 0){
        return 1;                                   // x^0 = 1.
    }
    if(exponent%2 == 0){
        lint ans = modex(base,exponent/2,mod);      // as, a^b = (a^(b/2))*(a^(b/2)), we use recursion to effectively reduce the no.
        return (ans*ans)%mod;                       // of operations required for computing.
    }
    return ((base%mod)*(modex(base,exponent-1,mod)))%mod;   // when exponent is odd, we reduce it by 1 to make it even, which again uses
}                                                           // recursion for computation.

ll modular_pow(ll base, ll exponent, int modulus);
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

    int t1, t2, t3, t4, mm = INT32_MAX - 5, t11, t12, t13, t14, h2 = 0;
    for (di = 0, iterAi = itE; (di < d) && (iterAi != itS); --iterAi, di++) {
        if (m == mm){
            t1 = power(m, di, M);     // (m^di)% M
             t11 = pow(m, di);     // (m^di)% M
            t12 = t11 % M;     // (m^di)% M
            // should t12 == t1?
        }
        else {
            t1 = pow(m, di);        // m^di
            t12 = t1;     // (m^di)% M
        }
        t3 = (*iterAi) * t1;
        t13 = (*iterAi) * t12;
        // should t3 == t13?
        h += t3;
        h2 += t13;

//        h += (*iterAi) * (((int) pow(m, di)) % M) % M;
//        h += (*iterAi) * (int) pow(m, di) % M;
    }
    int h10, h20, h11, h21;
    h10 = h % M;
    h11 = power(h, 1, M);
    h20 = h2 % M;
//    h21 = pow(h2, 1, M);

    return h11;
}


/* For exponentiation, use the binary left-to-right algorithm
 * unless the exponent contains more than FIVEARY_CUTOFF digits.
 * In that case, do 5 bits at a time.  The potential drawback is that
 * a table of 2**5 intermediate results is computed.
 */

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

//            perror("a%c < 0\n");




/*

 * Function to calculate modulus of x raised to the power y

 */

ll modular_pow(ll base, ll exponent, int modulus)

{

    ll result = 1;

    while (exponent > 0)

    {

        if (exponent % 2 == 1)

            result = (result * base) % modulus;

        exponent = exponent >> 1;

        base = (base * base) % modulus;

    }

    return result;

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

    /* if modulus == 1: return 0 */
    if (c == 1) return 0;
    /* Reduce base by modulus in some cases:
       1. If base < 0.  Forcing the base non-negative makes things easier.
       2. If base is obviously larger than the modulus.  The "small
          exponent" case later can multiply directly by base repeatedly,
          while the "large exponent" case multiplies directly by base 31
          times.  It can be unboundedly faster to multiply by
          base % modulus instead.
       We could _always_ do this reduction, but l_divmod() isn't cheap,
       so we only do it when it buys something. */
//    if (a < 0 || a > c) {
//        if ((temp = a % c) < 0){
//            temp = 0 - a;
//        }
//        a = temp;
//    }

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
