
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include "Point.h"

struct ComparatorqPoint {
    bool operator()(const qPoint *a, const qPoint *b) {
        return a->getDistance() < b->getDistance();
    }
};

std::vector<std::string> split(const std::string& s, char delimiter);

std::vector<int> splitInt(const std::string& s, char delimiter);

const Point * splitToPoint(const std::string& s, char delimiter);

//double manhattanDistance( std:: vector<int> , std:: vector<int> );

template <typename RT, typename DT >
RT manhattanDistance(DT point1, DT point2) {
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
