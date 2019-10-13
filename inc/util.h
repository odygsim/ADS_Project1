
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include "Point.h"

std::vector<std::string> split(const std::string& s, char delimiter);

std::vector<int> splitInt(const std::string& s, char delimiter);

const Point * splitToPoint(const std::string& s, char delimiter);
double manhattanDistance( std:: vector<int> , std:: vector<int> );

std::list < const qPoint * > exactKNN(std::vector<const Point *> dataList, const Point *queryPoint, int radius);
//std::list<const qPoint *> exactKNN(std::vector<const Point *> dataList, const Point *queryPoint, unsigned int radius);

int meanDistanceBetweenPoints(std::vector<const Point *> dataList);

const qPoint * AproximateNN(std::vector<const Point *> dataList, const Point *queryPoint /*TODO put here 3rd arg hash method lsh/cube*/);

std::vector<const Point *> readData(std::string const & fileName);
