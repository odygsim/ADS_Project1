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


std::string calculateStats(std::list<double> &distanceListEA, std::list<double> &timeListE, double & accuracy) {
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
    IterListDouble distanceListEnd = distanceListEA.end(), timeListEnd = timeListE.end(), iterDistanceList, iterTimeList;

    for (iterDistanceList = distanceListEA.begin(), iterTimeList = timeListE.begin();
         (iterDistanceList != distanceListEnd); ++iterDistanceList, ++iterTimeList) {
        sum += *iterTimeList;
        if (*iterDistanceList > maxDistance) maxDistance = *iterDistanceList;
    }
    meanTimeA = sum / timeListE.size();

    result += std::to_string(maxDistance)
              + "," + std::to_string(meanTimeA) + "," + std::to_string(accuracy);
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

