//
// Created by user on 10/12/19.
//

#include "../inc/test_util/Point.h"

#include <utility>

std::vector<int> Point::getList() const {
    return this->dataList;
}

std::string Point::getName() const { return this->name; }

Point::Point(std::string n, std::vector<int> l) {
    name = std::move(n);
    dataList = std::move(l);
}

qPoint::qPoint(std::string n, int d) {
    this->name = n;
    this->distance = d;
}

//bool qPoint::operator<( const qPoint & qPointObjB) const {
//    return distance < qPointObjB.distance;
//}

int qPoint::getDistance() const { return distance; }

std::string qPoint::getName() const { return name; }
