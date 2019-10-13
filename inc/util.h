
#include <vector>
#include <string>
#include <iostream>
#include "Point.h"

std::vector<std::string> split(const std::string& s, char delimiter);

std::vector<int> splitInt(const std::string& s, char delimiter);

Point * splitToPoint(const std::string& s, char delimiter);
int manhattanDistance( std:: vector<int> , std:: vector<int> );

qPoint * exactKNN(std::vector<const Point *> , const Point *);

std::vector<const Point *> copyData(std::string const & fileName);
