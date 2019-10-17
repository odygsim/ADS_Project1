// this file will contain general used utilities
//#include <tuple>
//#include <getopt.h>
#include <cmath>
#include <csignal>
#include "../inc/util.h"
//#include "../inc/FunctionH.h"
//#include "../inc/LSH_HT.h"

//template <typename RT, typename DT >
//RT manhattanDistance(DT point1, DT point2) {
//    RT sum = 0;
//
//    typename DT::iterator e1 = point1.end();
//    typename DT::iterator e2 = point2.end();
//    typename DT::iterator it1;
//    typename DT::iterator it2;
//    for (it1 = point1.begin(), it2 =point2.begin(); (it1 != e1) && (it2 != e2); ++it1, ++it2) {
//        sum += abs(*it1 - *it2);
//    }
//
//    return sum;
//}

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

const Point *splitToPoint(const std::string &s, char delimiter) {
    std::vector<int> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    int i = 0;
    std::string name;
    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            if (i == 0)
                name = token;
            else
                tokens.push_back(stoi(token));
        }
        i += 1;
    }
    return new Point(name, tokens);
}

//int meanDistanceBetweenPoints(std::vector<const Point *> & dataList) {
//    /* For Each Point calculate its nearest neighbor
//     * bruteforce, run each point versus all dataset
//     * and produce the mean results*/
//    std::list<const int> minDistances;
//    int sum = 0;
//
//
//    for (unsigned long i = 0; i < dataList.size(); ++i) {
//        std::list<const qPoint *> distanceList;
//        for (unsigned long j = 0; j < dataList.size(); ++j) {
//            distanceList.push_back(new qPoint(dataList[j]->getName(),
//                                              manhattanDistance<int,std::vector<int>>(dataList[j]->getList(), dataList[i]->getList())));
//        }
//        // Get min
//        distanceList.sort(ComparatorqPoint());
//        minDistances.push_back(distanceList.front()->getDistance());
//    }
//    // Calculate mean
//    for (auto i: minDistances) {
//        sum += i;
//    }
//    return sum / (int) minDistances.size();
//}

//std::list < const qPoint * > exactKNN(std::vector<const Point *> & dataList, const Point *queryPoint, int radius) {
//    typedef std::list<const qPoint *> qpList;
//    qpList distanceList;
//    qpList returnList;
//    typedef qpList::iterator qpIt;
//
//    int j;
//
//    for (unsigned long i = 0; i < dataList.size(); ++i)
//        distanceList.push_back(
//                new qPoint(dataList[i]->getName(), manhattanDistance<int,std::vector<int>>(dataList[i]->getList(), queryPoint->getList())));
//    distanceList.sort(ComparatorqPoint());
//
//     qpIt itS = distanceList.begin();
//     qpIt it;
//     qpIt itE = distanceList.end();
//    for (j = 0, it = itS; (j < radius) && (it!=itE); ++j, ++it) {
//        returnList.push_back(*it);
//        }
////    it = distanceList.begin();
////    for (it = itS; (it!=itE); ++it) {
////        std::cout << "Name: " << (*it)->getName()  << ", " << (*it)->getDistance() << std:: endl;//.push_back(*it);
////    }
////    qPoint *ret = distanceList.front();
////    for (int j = 0; j < distanceList.size(); ++j) {
////        delete distanceList[i];
////    }
//    return returnList;
//}

const qPoint *AproximateKNN(const std::vector<const Point *> &dataList,
                            const Point *queryPoint /*TODO put here 3rd arg hash method lsh/cube*/) {
    std::list<const qPoint *> distanceList;
//    LSH lsh(3000, dataList[0]->getList().size());

//    for (unsigned long i = 0; i < dataList.size(); ++i)
//        lsh.addPoint()

//        distanceList.push_back(
//                new qPoint(dataList[i]->getName(), manhattanDistance(dataList[i]->getList(), queryPoint->getList())));
//    distanceList.sort(ComparatorqPoint());

    return distanceList.front();
}

std::vector<const Point *> readData(const std::string &fileName) {
    std::vector<const Point *> dataList;
    std::ifstream file;
    std::string tmp;

    // Check filenames
    if (file.fail()) {
        std::cerr << "Error: Wrong filename / notFound" << std::endl;
        exit(1);
    }
    // Test if file can be opened
    file.open(fileName);
    if (!file) {
        std::cerr << "Unable to open file : " << fileName << std::endl;
        exit(1);
    }
    // Iterate over input file and store each Point's dimension data in a vector
    while (!file.eof()) {
        getline(file, tmp, '\r'); // remove /r
        if (tmp[0] == '\n')
            tmp.erase(tmp.begin());
        if (!tmp.empty())
            dataList.push_back(splitToPoint(tmp, ' '));
    }
    file.close();
    return dataList;
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
