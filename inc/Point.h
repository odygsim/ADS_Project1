//
// Created by user on 10/12/19.
//
#include <string>
#include <vector>

#ifndef ADS_PROJECT1_POINT_H
#define ADS_PROJECT1_POINT_H


class Point {
    std::string name;
    std::vector<double> dList;

    std::vector <double> getList();
    void setPoint();
public:
    Point(std::string, std::vector<double>);
};


#endif //ADS_PROJECT1_POINT_H