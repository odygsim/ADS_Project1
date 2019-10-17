
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
#include "Point.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <values.h>
#include <chrono>

int pow(int a, int b, int c);

std::string getDatetime(bool);

std::string getFilename(const std::string &str);

struct ComparatorqPoint {
    bool operator()(const qPoint *a, const qPoint *b) {
        return a->getDistance() < b->getDistance();
    }
};

template<int index>
struct TupleLess {
    template<typename Tuple>
    bool operator()(const Tuple &left, const Tuple &right) const {
        return std::get<index>(left) < std::get<index>(right);
    }
};


std::vector<std::string> split(const std::string &s, char delimiter);

std::vector<int> splitInt(const std::string &s, char delimiter);

const Point *splitToPoint(const std::string &s, char delimiter);

//double manhattanDistance( std:: vector<int> , std:: vector<int> );

template<typename RT, typename DT>
RT manhattanDistance(DT &point1, DT &point2) {
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

std::list<const qPoint *> exactKNN(std::vector<const Point *> &dataList, const Point *queryPoint, int radius);
//std::list<const qPoint *> exactKNN(std::vector<const Point *> dataList, const Point *queryPoint, unsigned int radius);

int meanDistanceBetweenPoints(std::vector<const Point *> dataList);

const qPoint *AproximateNN(std::vector<const Point *> dataList,
                           const Point *queryPoint /*TODO put here 3rd arg hash method lsh/cube*/);

std::vector<const Point *> readData(std::string const &fileName);

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
    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            if (i == 0)
                name = token;
            else // TODO make prediction for double? or get a parameter what to read? int/double
                tokens.push_back(stoi(token));
        }
        i += 1;
    }
    dataList.push_back(tokens);
    labelList.push_back(name);
}

template<class CX, class CY, class X, class Y>
//data container , data labels
bool readDataAndLabelsFromFile2(const std::string &fileName, CX &fDataList, CY &fLabelList) {
    /**
     * @brief Read data and label to objects CX and CY accordingly.
     * @param fileName string filename to read.
     * @param fDataList The object that the data will be stored.
     * @param fLabelList The object that the labels will be stored.
     * @return The fLabelList, fDataList, by parameter and int (data read).
     */
    std::ifstream file;
    std::string tmp;
    char space = ' ';

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
            splitToPoint3<CX, CY, X, Y>(tmp, space, fDataList, fLabelList);
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

int readHypercubeParameters(int argc, char **argv,
                            std::string &inputFile, std::string &queryFile, std::string &outputFile,
                            int &k, int &M, int &probes);


std::string calculateStats(std::list<double> &distanceListEA, std::list<double> &timeListE);

template<class Y, class D, class TY>
std::tuple<std::string, std::string>
unrollResult(std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> & listExact,
             std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> & listAprox, TY & y) {
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


    for (itY = y.begin(), itListEx = listExact.begin(), itListAp = listAprox.begin();
         (itY != yE); ++itY, ++itListEx, ++itListAp) {
//        std::tuple<double, std::list<std::tuple<Y, D>> currentTuple
// Get for each tuple second arg the list of points
        listTuples curLE = std::get<1>(*itListEx);
        listTuples curLA = std::get<1>(*itListAp);

        result += "Query: " + *(itY) + delim;
        result += "Nearest neighbor: " + std::get<0>(curLA.front()) + delim;
        distanceE = std::get<1>(curLE.front());
        distanceA = std::get<1>(curLA.front());
        maxDistanceAdivE.push_back(distanceA / distanceE);
        result += "distanceLSH: " + std::to_string(distanceA) + delim;
        result += "distanceTrue: " + std::to_string(distanceE) + delim;
////            sprintf(str, "%.2f", result->getDistance());
////            string a(str, strlen(str));
////            output += "distanceTrue: " ;
////            output += a + space;// std::to_string(result->getDistance()) + "\n";
        timeA = std::get<0>(*itListAp);
        meanTimeA.push_back(timeA);
        result += "tLSH: " + std::to_string(timeA) + delim;
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
//        if (i == 3) break;
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

#endif //ADS_PROJECT1_UTIL_H