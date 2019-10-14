
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <tuple>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include "Point.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

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


std::vector<std::string> split(const std::string& s, char delimiter);

std::vector<int> splitInt(const std::string& s, char delimiter);

const Point * splitToPoint(const std::string& s, char delimiter);

//double manhattanDistance( std:: vector<int> , std:: vector<int> );

template <typename RT, typename DT >
RT manhattanDistance(DT & point1, DT & point2) {
    RT sum = 0;

    typename DT::iterator e1 = point1.end();
    typename DT::iterator e2 = point2.end();
    typename DT::iterator it1;
    typename DT::iterator it2;
    for (it1 = point1.begin(), it2 =point2.begin(); (it1 != e1) && (it2 != e2); ++it1, ++it2) {
        sum += abs(*it1 - *it2);
    }

    return sum;
}

std::list < const qPoint * > exactKNN(std::vector<const Point *> & dataList, const Point *queryPoint, int radius);
//std::list<const qPoint *> exactKNN(std::vector<const Point *> dataList, const Point *queryPoint, unsigned int radius);

int meanDistanceBetweenPoints(std::vector<const Point *> dataList);

const qPoint * AproximateNN(std::vector<const Point *> dataList, const Point *queryPoint /*TODO put here 3rd arg hash method lsh/cube*/);

std::vector<const Point *> readData(std::string const & fileName);

template < class X, class Y>
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
            else
                tokens.push_back(stoi(token));
        }
        i += 1;
    }
    return std::make_tuple(name,tokens);
}

//template <class X, class Y>
//std::tuple<Y, X, typename std::enable_if<std::__is_floating> splitToPoint2(const std::string &s, char delimiter) {
//std::tuple<std::string, std::vector<double>> splitToPoint2(const std::string &s, char delimiter) {
////    std::tuple<std::string, std::vector<double>> splitToPoint2(const std::string &s, char delimiter) {
//    std::vector<double> tokens;
////    std::tuple<std::string, std::vector<int>> tuple1;
//    std::string token;
//    std::istringstream tokenStream(s);
//    int i = 0;
//    std::string name;
//    while (std::getline(tokenStream, token, delimiter)) {
//        if (!token.empty()) {
//            if (i == 0)
//                name = token;
//            else
//                tokens.push_back(stod(token));
//        }
//        i += 1;
//    }
//    return std::make_tuple(name,tokens);
//}

template < class Y, class X>
std::vector<std::tuple<Y,X>> readData2(const std::string &fileName) {
    std::vector<std::tuple<Y,X>> dataList;
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
            dataList.push_back(splitToPoint2<X,Y>(tmp, ' '));
    }
    file.close();
    return dataList;
}

void print_cube_usage();
int readHypercubeParameters(int argc, char** argv,
                            std::string &inputFile, std::string&queryFile, std::string &outputFile,
                            int &k, int &M, int &probes);

template<class Y, class D, class TY>
std::string
unrollResult(std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> listExact,
             std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> listAprox, TY y) {

    typedef std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> Ltl;
    typedef std::list<std::tuple<Y, D>> listTuples;
    std::string result, delim = " ";
//    typedef std::list<std::tuple<double, std::list<Y, D>>> Ltl;
    typename TY::iterator yE = y.end();
    typename Ltl::iterator lE1 = listExact.end();
    typename Ltl::iterator lE2 = listAprox.end();
    typename TY::iterator itY;
    typename Ltl::iterator itListEx;
    typename Ltl::iterator itListAp;
    for (itY = y.begin(), itListEx = listExact.begin(), itListAp = listAprox.begin();
         (itY != yE); ++itY, ++itListEx, ++itListAp) {
//        std::tuple<double, std::list<std::tuple<Y, D>> currentTuple
// Get for each tuple second arg the list of points
        listTuples curLE = std::get<1>(*itListEx);
        listTuples curLA = std::get<1>(*itListAp);

        result += "Query: " + *(itY) + delim;
        result += "Nearest neighbor: " + std::get<0>(curLE.front()) + delim;
        result += "distanceLSH: " + std::to_string(std::get<1>(curLE.front())) + delim;
        result += "distanceTrue: " + std::to_string(std::get<1>(curLA.front())) + delim;
////            sprintf(str, "%.2f", result->getDistance());
////            string a(str, strlen(str));
////            output += "distanceTrue: " ;
////            output += a + space;// std::to_string(result->getDistance()) + "\n";
        result += "tLSH: " + std::to_string(std::get<0>(*itListAp)) + delim;
        result += "tTrue: " + std::to_string(std::get<0>(*itListEx)) + delim;
        result += "R-near neighbors:\n";
        for (auto item : curLE) {
            // for each query
            result += std::get<0>(item) + delim;
        }
        result += delim;
//            cout << result << endl;
//            result = "";
    }
}
