//
// Created by user on 10/12/19.
//
#include <string>
#include <vector>

#ifndef ADS_PROJECT1_POINT_H
#define ADS_PROJECT1_POINT_H


class Point {
    std::string name;
    std::vector<int> dataList;

public:
    Point(std::string, std::vector<int> );
    ~Point()= default;;
    std::vector <int> getList() const;
    std::string getName() const;
//    void setPoint();
};

class qPoint {
    std::string name;
    int distance;
public:
    qPoint(std::string, int);
    ~qPoint()= default;;
//    bool operator <(const qPoint & qPointObj) const;
    std::string getName() const;
    int getDistance() const;
};

#endif //ADS_PROJECT1_POINT_H
