
#include <vector>
#include <string>
#include <iostream>
#include "Point.h"

std::vector<std::string> split(const std::string& s, char delimiter);

std::vector<int> splitInt(const std::string& s, char delimiter);

Point * splitToPoint(const std::string& s, char delimiter);
