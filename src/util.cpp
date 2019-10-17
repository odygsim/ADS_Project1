// this file will contain general used utilities
#include <cmath>
#include <csignal>
#include "../inc/util.h"

std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}


std::vector<int> splitInt(const std::string &s, char delimiter) {
    std::vector<int> tokens;
    std::string token;
    size_t *pEnd = nullptr;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(stoi(token));
    }
    return tokens;
}


void print_cube_usage() {
    fprintf(stderr, "Usage: cube -d <input file> -q <query file> k <int> -M <int> -probes <int> -o <output file>\n");
}

int readHypercubeParameters(int argc, char **argv,
                            std::string &inputFile, std::string &queryFile, std::string &outputFile,
                            int &k, int &M, int &probes) {

    extern char *optarg;
    int opt = 0;

    //Set default parameters values
    k = 3;
    M = 10;
    probes = 2;

    //Specify expected options
    static struct option cube_options[] = {
            {"d",      required_argument, 0, 'd'},
            {"q",      required_argument, 0, 'q'},
            {"o",      required_argument, 0, 'o'},
            {"k",      required_argument, 0, 'k'},
            {"M",      required_argument, 0, 'M'},
            {"probes", required_argument, 0, 'p'}
    };

    int longIndex = 0;
    int dflag = 0, qflag = 0, oflag = 0;

    // Get options given
    while ((opt = getopt_long_only(argc, argv, "",
                                   cube_options, &longIndex)) != -1) {
        switch (opt) {
            case 'd' :
                dflag = 1;
                inputFile = optarg;
                break;
            case 'q' :
                qflag = 1;
                queryFile = optarg;
                break;
            case 'o' :
                oflag = 1;
                outputFile = optarg;
                break;
            case 'k' :
                k = atoi(optarg);
                break;
            case 'M' :
                M = atoi(optarg);
                break;
            case 'p' :
                probes = atoi(optarg);
                break;
            default:
                print_cube_usage();
                exit(EXIT_FAILURE);
        }
    }

    //  Check if necessary options have been provided
    if (dflag == 0) {
        fprintf(stderr, "cube: missing -d option\n");
        print_cube_usage();
        exit(EXIT_FAILURE);
    } else if (qflag == 0) {
        fprintf(stderr, "cube: missing -q option\n");
        print_cube_usage();
        exit(EXIT_FAILURE);
    } else if (oflag == 0) {
        fprintf(stderr, "cube: missing -o option\n");
        print_cube_usage();
        exit(EXIT_FAILURE);
    }

    return 0;
}


std::string calculateStats(std::list<double> &distanceListEA, std::list<double> &timeListE) {
    /**
     * @brief Calculate the statistics A) max (distA / distanceE), B) meanTimeA
     * @param distanceList A list that contains distA/distE double numbers
     * @param timeList A list that contains all Approx time double numbers
     * @return string with result unrolled.
     * @return string with result.
     */
    double sum = 0, meanTimeA, maxDistance = MINDOUBLE;
    std::string result;

    typedef std::list<double>::iterator IterListDouble;
    IterListDouble distanceListEnd = distanceListEA.end();
    IterListDouble timeListEnd = timeListE.end();
    IterListDouble iterDistanceList;
    IterListDouble iterTimeList;
    for (iterDistanceList = distanceListEA.begin(), iterTimeList = timeListE.begin();
         (iterDistanceList != distanceListEnd); ++iterDistanceList, ++iterTimeList) {
        sum += *iterTimeList;
        if (*iterDistanceList > maxDistance) maxDistance = *iterDistanceList;
    }
    meanTimeA = sum / timeListE.size();

    result += std::to_string(maxDistance)
              + "," + std::to_string(meanTimeA);
    return result;
}


std::string getDatetime(bool forFile = false) {
    /**
     * @brief Return current datetime in file or print in ISO format.
     * @param forFile boolean to return print or file format
     * @return string with datetime.
     */
    time_t secs = time(0);
    tm *t = localtime(&secs);
    char buff[40];
    if (forFile) {
        sprintf(buff, "%04d_%02d_%02d__%02d_%02d_%02d",
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                t->tm_hour, t->tm_min, t->tm_sec);
    } else {
        sprintf(buff, "%04d-%02d-%02d %02d:%02d:%02d",
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                t->tm_hour, t->tm_min, t->tm_sec);
    }
////This prints the date in ISO format.
    return std::string(buff);
}

std::string getFilename(const std::string &str) {
    unsigned found = str.find_last_of("/\\");
//    std::cout << " path: " << str.substr(0,found) << '\n';
    return str.substr(found + 1);
}


std::chrono::steady_clock::time_point initTime() { /**@return a time point*/return std::chrono::steady_clock::now(); }

double getElapsed(std::chrono::steady_clock::time_point start) {
    /**
     * @brief Calculates time elapsed in seconds since start.
     * @param start The start time point.
     * @return elapsed time.
     */
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0;
}

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

/* For exponentiation, use the binary left-to-right algorithm
 * unless the exponent contains more than FIVEARY_CUTOFF digits.
 * In that case, do 5 bits at a time.  The potential drawback is that
 * a table of 2**5 intermediate results is computed.
 */

static char * _digits_of_n(int n, int b) {
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

    return z;
}

void scanRadius(const std::string &s, double &radius, char &delimiter) {
    /**
     * @brief Parses first line looking for format of file.
     * @param s string to parse.
     * @param radius Variable to return radius.
     * @param delimiter The delimiter to split
     */
    std::string token;
    std::istringstream tokenStream(s);
    if (std::getline(tokenStream, token, delimiter) && !token.empty())              // try get radius
    {
        if (token[0] == 'R' && token[token.size() - 1] == ':') { // get radius
            if (std::getline(tokenStream, token, delimiter) && !token.empty()) // get radius
                radius = stod(token);
        }
    }
}
