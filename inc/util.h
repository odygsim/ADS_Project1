#ifndef ADS_PROJECT1_UTIL_H
#define ADS_PROJECT1_UTIL_H

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <tuple>
#include <vector>
#include <list>
#include <string>
#include <clocale>
#include <cstdio>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>
#include <values.h>
#include <chrono>
#include <cstring>
#include <assert.h>
#include <algorithm>
#include <unordered_map>
//#include "Point.h"

#define __STDC_WANT_LIB_EXT1__ 1
#define FIVEARY_CUTOFF 8
#define lint long long int
#define ll long long
#define S second

std::string getDatetime(bool);

std::string getFilename(const std::string &str);


template<int index>
struct TupleLess {
    template<typename Tuple>
    bool operator()(const Tuple &left, const Tuple &right) const {
        return std::get<index>(left) < std::get<index>(right);
    }
};


std::vector<std::string> split(const std::string &s, char delimiter);

template<typename RT, typename DT>
RT manhattanDistance(DT &point1, DT &point2) {
    /**
     * @brief calculates l1 distance from given vectors/lists.
     * @point1 An object that has a point with a big dimension.
     * @point2 An object that has a point with a big dimension.
     * @return the sum of l1 distance.
     */
    RT sum = 0;

    typename DT::iterator e1 = point1.end();
    typename DT::iterator e2 = point2.end();
    typename DT::iterator it1;
    typename DT::iterator it2;
    for (it1 = point1.begin(), it2 = point2.begin(); (it1 != e1) && (it2 != e2); ++it1, ++it2) {
        sum += abs(*it1 - *it2);
    }

    return sum;
}

template<typename RT, typename DT>
RT euclideanDistance(DT &point1, DT &point2) {
    /**
     * @brief calculates l1 distance from given vectors/lists.
     * @point1 An object that has a point with a big dimension.
     * @point2 An object that has a point with a big dimension.
     * @return the sum of l1 distance.
     */
    RT sum = 0;

    typename DT::iterator e1 = point1.end();
    typename DT::iterator e2 = point2.end();
    typename DT::iterator it1;
    typename DT::iterator it2;
    for (it1 = point1.begin(), it2 = point2.begin(); (it1 != e1) && (it2 != e2); ++it1, ++it2) {
        sum += (*it1-*it2)*(*it1 - *it2);
    }

    return sqrt(sum);
}

template<class X, class Y>
std::tuple<Y, X> splitToPoint2(const std::string &s, char delimiter) {
    X tokens;
    std::string token;
    std::istringstream tokenStream(s);
    int i = 0;
    std::string name;
    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            if (i == 0)
                name = token;
            else // TODO make prediction for double? or get a parameter what to read? int/double
                tokens.push_back(stoi(token));
        }
        i += 1;
    }
    return std::make_tuple(name, tokens);
}

template<class CX, class CY, class X, class Y>
void splitToPoint3(const std::string &s, char delimiter, CX &dataList, CY &labelList) {
    /**
     * @brief Parse data and label to objects CX and CY accordingly.
     * @param s string to parse.
     * @param dataList The object that the data will be stored.
     * @param labelList The object that the labels will be stored.
     * @return The fLabelList, fDataList, by parameter.
     */
    std::ifstream file;
    X tokens;
    std::string token;
    std::istringstream tokenStream(s);
    int i = 0;
    std::string name;
    if (std::getline(tokenStream, token, delimiter) && !token.empty()) {
        name = token;
    }
    while (std::getline(tokenStream, token, delimiter) && !token.empty()) {
        tokens.push_back(stoi(token));
    }
    dataList.push_back(tokens);
    labelList.push_back(name);
}


template<class CX, class CY, class X, class PrimitiveType>
void splitToCurve(char *str, CX &dataList, CY &labelList) {
    /**
     * @brief Parse data and label to objects CX and CY accordingly.
     * @param str string to parse.
     * @param dataList The object that the data will be stored.
     * @param labelList The object that the labels will be stored.
     * @return The fLabelList, fDataList, by parameter.
     */
    X tokens;
    double x = 0, y = 0;
    int tokens_size = 0;
    char *found;
    found = strchr(str, '(');

    char name[10];
    char *tokenC;
    const char LP[2] = ")";
    const char RP[4] = ") (";
    // get first token
    sscanf(str, "%s\t%d\t", name, &tokens_size);
    tokenC = strtok(++found, LP); // done need it
    sscanf(tokenC, "%lf, %lf", &x, &y);
    std::vector<PrimitiveType> token;
    token.push_back(x);
    token.push_back(y);
    tokens.push_back(token);
    token.clear();
    tokens.reserve(tokens_size);
    while (tokenC != NULL && tokenC[0] != '\n') {
        tokenC = strtok(NULL, LP);
        sscanf(tokenC, " (%lf, %lf", &x, &y);
//        std::vector<X> token;
        token.push_back(x);
        token.push_back(y);
        tokens.push_back(token);
        token.clear();
    }
    dataList.push_back(tokens);
    labelList.push_back(std::string(name));
}

template<class CX, class CY, class X, class Y>
void readTrajectories(const std::string &filename, CX &dataList, CY &labelList) {
    /**
     * @brief Reads Trajectories into 2 lists data and label.
     * @param filename to parse.
     * @param dataList An object of type list<vector<tuple<double,double>>
     * @param labelList An object of type list<string>
     *
     */
    FILE *fp = fopen(filename.c_str(), "r");
    if (fp == NULL){
        std::cerr << "File: " + filename +  " was not found.\n";
        exit(EXIT_FAILURE);
    }
    char *line = NULL;
    size_t len = 0;
    while ((getline(&line, &len, fp)) != -1) {
        splitToCurve<CX, CY, X, Y>(line, dataList, labelList);
    }
    fclose(fp);
    if (line)
        free(line);
}


void scanRadius(const std::string &s, double &radius, char &delimiter);

template<class CX, class CY, class X, class Y>
//data container , data labels
bool readDataAndLabelsFromFile2(const std::string &fileName, CX &fDataList, CY &fLabelList, double radius = 0.0) {
    /**
     * @brief Read data and label to objects CX and CY accordingly.
     * @param fileName string filename to read.
     * @param fDataList The object that the data will be stored.
     * @param fLabelList The object that the labels will be stored.
     * @return The fLabelList, fDataList, by parameter and int (data read).
     */
    std::ifstream file;
    std::string tmp;
    char delim = ' ', newlineUnix = '\n', newlineWindows = '\r';

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
    if (!file.eof()) {
        // Do checks for Radius: , split character. which file i read..
        getline(file, tmp, newlineUnix);
        if (!tmp.empty() && (tmp[tmp.size() - 1] == newlineWindows))
            tmp.erase(tmp.end() - 1); // remove /r
        scanRadius(tmp, radius, delim);
        if (radius != 0.0) // no radius
        { // go to next line
        } else
            splitToPoint3<CX, CY, X, Y>(tmp, delim, fDataList, fLabelList);
    }
    // we have read the radius go on

    // Iterate over input file and store each Point's dimension data in a vector
    while (!file.eof()) {
        getline(file, tmp, newlineUnix);
        if (!tmp.empty()) {
            if ( tmp[tmp.size() - 1] == newlineWindows)
                tmp.erase(tmp.end() - 1); // remove /r
            splitToPoint3<CX, CY, X, Y>(tmp, delim, fDataList, fLabelList);
        }
    }
    file.close();
    return true;

}

template<class Y, class X>
std::vector<std::tuple<Y, X>> readData2(const std::string &fileName) {
    std::vector<std::tuple<Y, X>> dataList;
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
            dataList.push_back(splitToPoint2<X, Y>(tmp, ' '));
    }
    file.close();
    return dataList;
}

void print_cube_usage();

int
readHypercubeParameters(int argc, char **argv, std::string &inputFile, std::string &queryFile, std::string &outputFile,
                        int &k, int &M, int &probes);


std::string calculateStats(std::list<double> &distanceListEA, std::list<double> &timeListE);

template<class Y, class D, class TY>
std::tuple<std::string, std::string> unrollResult(std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> &listExact,
                                                  std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> &listAprox,
                                                  TY &y, const std::string algName = "LSH") {
    /**
     * @brief Unroll the results of Approximate and Exact Algorithms and calculate some numbers for statistics.
     * @param listExaxt The list that contains objects of (timeValue, (label, distance)).
     * @param listAprox The list that contains objects of (timeValue, (label, distance)).
     * @return string with result unrolled and stats string as tuple.
     */

    typedef std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> Ltl;
    typedef std::list<std::tuple<Y, D>> listTuples;
    std::string result, stats, delim = " ";
//    typedef std::list<std::tuple<double, std::list<Y, D>>> Ltl;
    typename TY::iterator yE = y.end();
    typename Ltl::iterator lE1 = listExact.end();
    typename Ltl::iterator lE2 = listAprox.end();
    typename TY::iterator itY;
    typename Ltl::iterator itListEx;
    typename Ltl::iterator itListAp;
    std::list<double> meanTimeA;
    std::list<double> maxDistanceAdivE;
    double timeA, questionA, questionB;
    D distanceA, distanceE;
    std::string timeApproxName = "t" + algName + ": ";
    std::string distanceApproxName = "distance" + algName + ": ";


    for (itY = y.begin(), itListEx = listExact.begin(), itListAp = listAprox.begin(); (itY !=
                                                                                       yE); ++itY, ++itListEx, ++itListAp) {
// Get for each tuple second arg the list of points
        listTuples curLE = std::get<1>(*itListEx);
        listTuples curLA = std::get<1>(*itListAp);
        assert(!curLA.empty());
        result += "Query: " + *(itY) + delim;
        result += "Nearest neighbor: " + std::get<0>(curLA.front()) + delim;
        distanceE = std::get<1>(curLE.front());
        distanceA = std::get<1>(curLA.front());
        maxDistanceAdivE.push_back(distanceA / distanceE);
        result += distanceApproxName; result += std::to_string(distanceA) + delim;
        result += "distanceTrue: " + std::to_string(distanceE) + delim;
////            sprintf(str, "%.2f", result->getDistance());   // these are commented in case
                                                               /// we need the double with less digits.
////            string a(str, strlen(str));
////            output += "distanceTrue: " ;
////            output += a + space;// std::to_string(result->getDistance()) + "\n";
        timeA = std::get<0>(*itListAp);
        meanTimeA.push_back(timeA);
        result += timeApproxName; result += std::to_string(timeA) + delim;
        result += "tTrue: " + std::to_string(std::get<0>(*itListEx)) + delim;
        if (curLA.size() > 1) {
            result += "R-near neighbors:\n";
            for (auto item : curLA) {
                // for each query
                result += std::get<0>(item) + delim;
            }
        }
        result += "\n";
    }
    stats = calculateStats(maxDistanceAdivE, meanTimeA);

    std::cout << result << std::endl;
    std::cout << stats << std::endl;

    return std::pair<std::string, std::string>(result, stats);
}

template<class TD, class TID, class D>
int meanDistanceBetweenPoints(TD &data) {
    /**
     * @brief General fit for all methods lsh, cube, exactKnn.
     * @param x The first object with data.
     * @param y The second object with label.
     * @return void.
     *
     *  For Each Point calculate its nearest neighbor
     * bruteforce, run each point versus all dataset
     * and produce the mean results
     */

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::list<D> minDistances;
    int sum = 0, i = 0, size = data.size();
    double elapsed = 0;
    typedef typename TD::iterator tdIt; // Iterator on the list of vectors
    tdIt iteratorData; // Init Iterator on list of vectors
    tdIt iteratorDataJ; // Init Iterator on list of vectors
    tdIt itDE = data.end(); // end of data iterator
    for (iteratorData = data.begin(); iteratorData != itDE; ++iteratorData) {
        std::cout << getDatetime(false) + "\t\t\t\tChecking point: " << i << "/" << size << std::endl;
        std::list<D> distanceList;
        for (iteratorDataJ = data.begin(); iteratorDataJ != itDE; ++iteratorDataJ) {
            distanceList.push_back(manhattanDistance<D, TID>(*iteratorData, *iteratorDataJ));
        }
        i++;
        distanceList.sort(); // Get min
        distanceList.pop_front();
        minDistances.push_back(distanceList.front());
    }
    // Calculate mean
    for (auto j: minDistances) {
        sum += j;
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0;
    i = sum / minDistances.size();
    std::cout << "r: " << std::to_string(i) + ", Calculation Time : " << std::to_string(elapsed) << std::endl;
    return i;
}

std::chrono::steady_clock::time_point initTime();

double getElapsed(std::chrono::steady_clock::time_point start);


/// Test function for modulus exponentiation

ll modular_pow(ll base, ll exponent, int modulus);

int pow(int a, int b, int c);

lint modex(lint base, lint exponent, lint mod);


int power(int x, unsigned int y, int p);

//def getAccuracy(testSet, predictions):
//correct = 0
//for x in range(len(testSet)):
//if testSet[x][0] is predictions[x]:
//correct += 1
//return (correct / float(len(testSet))) * 100.0


std::list<unsigned  int> find_path(const unsigned char *path , int  m, int n);
/**
 *
 * @tparam X  is a vector<double> the point Type.
 */

template <typename CurveX, typename PointX, typename PrimitiveType>
std::tuple<double, std::list<std::tuple<int,int>>> dtw(CurveX & a, CurveX & b, PrimitiveType (*f) (PointX &, PointX &))
{
    /**
     * @brief Calculates dtw distance between 2 curves/lines and the path.
     * @param a First curve to compare.
     * @param b Second curve to compare.
     * @param f The metric method tha will be used to calculate distances.
     * @return A tuple of {double, list({int,int})} that are distance, path points i,j in a list.
     */

    using namespace std;
    const int m = a.size() ;
    const int n = b.size() ;
    double distance;
    typedef tuple<double, int, int> TUP; // The Array keeps a tuple of { currentCost, i Position, j Position}
    list<tuple<int, int>> Path;         // The Path is return in a list of tuples {i,j}
    int i, j;
    // Alloc 2-d array.
    vector<vector<TUP>> DTW(m, vector<TUP> (n, {MAXDOUBLE, 0, 0})); // The array m * n that hold the values calculated.

    DTW[0][0] = {0,0,0};                         // init 0 position with 0, the start pos.
    for (i = 1; i < m; ++i) {                   // For all rows
        for (j = 1; j < n; ++j) {               // For all columns
            distance = f(a[i - 1], b[j - 1]);         // Calculate Distance from point to point.
            // Add to a vector the 3 neighbors (i-1, j), (i, j-1) , (i-1,j-1) to sort them by cost.
            vector< TUP > TempVector{ {get<0>(DTW[i-1][j])+distance,i-1,j}, {get<0>(DTW[i][j-1])+distance,i,j-1},
                                      {get<0>(DTW[i-1][j-1])+distance,i-1,j-1}};
            sort(begin(TempVector), end(TempVector), TupleLess<0>());
              DTW[i][j] = TempVector[0];
              TempVector.clear();
        }
    }
    i = m-1 , j = n-1 ;
    while (i > 0 && j > 0) {
        Path.push_front({i-1,j-1});
        i = get<1>(DTW[i][j]);                  // Get i
        j = get<2>(DTW[i][j]);                  // get j
    }
        // get total distance, path and return them.
return  {get<0>(DTW[m-1][n-1]), Path};

}

#endif //ADS_PROJECT1_UTIL_H