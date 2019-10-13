// this file will contain general used utilities
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
//#include <tuple>
#include "../inc/util.h"
#include "../inc/FunctionH.h"

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


int meanDistanceBetweenPoints(std::vector<const Point *> & dataList) {
    /* For Each Point calculate its nearest neighbor
     * bruteforce, run each point versus all dataset
     * and produce the mean results*/
    std::list<const int> minDistances;
    int sum = 0;


    for (unsigned long i = 0; i < dataList.size(); ++i) {
        std::list<const qPoint *> distanceList;
        for (unsigned long j = 0; j < dataList.size(); ++j) {
            distanceList.push_back(new qPoint(dataList[j]->getName(),
                                              manhattanDistance<int,std::vector<int>>(dataList[j]->getList(), dataList[i]->getList())));
        }
        // Get min
        distanceList.sort(ComparatorqPoint());
        minDistances.push_back(distanceList.front()->getDistance());
    }
    // Calculate mean
    for (auto i: minDistances) {
        sum += i;
    }
    return sum / (int) minDistances.size();
}

std::list < const qPoint * > exactKNN(std::vector<const Point *> & dataList, const Point *queryPoint, int radius) {
    typedef std::list<const qPoint *> qpList;
    qpList distanceList;
    qpList returnList;
    typedef qpList::iterator qpIt;

    int j;

    for (unsigned long i = 0; i < dataList.size(); ++i)
        distanceList.push_back(
                new qPoint(dataList[i]->getName(), manhattanDistance<int,std::vector<int>>(dataList[i]->getList(), queryPoint->getList())));
    distanceList.sort(ComparatorqPoint());

     qpIt itS = distanceList.begin();
     qpIt it;
     qpIt itE = distanceList.end();
    for (j = 0, it = itS; (j < radius) && (it!=itE); ++j, ++it) {
        returnList.push_back(*it);
        }
//    it = distanceList.begin();
//    for (it = itS; (it!=itE); ++it) {
//        std::cout << "Name: " << (*it)->getName()  << ", " << (*it)->getDistance() << std:: endl;//.push_back(*it);
//    }
//    qPoint *ret = distanceList.front();
//    for (int j = 0; j < distanceList.size(); ++j) {
//        delete distanceList[i];
//    }
    return returnList;
}

const qPoint * AproximateKNN(std::vector<const Point *> dataList,
                           const Point *queryPoint /*TODO put here 3rd arg hash method lsh/cube*/) {
    std::list<const qPoint *> distanceList;

//    for (unsigned long i = 0; i < dataList.size(); ++i)
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

