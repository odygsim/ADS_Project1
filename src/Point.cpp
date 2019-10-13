//
// Created by user on 10/12/19.
//

#include "../inc/Point.h"

#include <utility>

std::vector<int> Point::getList() const {
    return this->dataList;
}

std::string Point::getName() const { return this->name; }

Point::Point(std::string n, std::vector<int> l) {
    name = std::move(n);
    dataList = std::move(l);
}

qPoint::qPoint(std::string n, double d) {
    this->name = n;
    this->distance = d;
}

//struct ComparatorqPoint{
//    bool operator() (qPoint const & qPointObjA, qPoint const & qPointObjB ) {
//        return qPointObjA.getDistance() < qPointObjB.getDistance();
//    }
////    bool operator() (qPoint * a, qPoint * b){ return a->getDistance() < b->getDistance();}
//};
//bool qPoint::operator <(qPoint const & qPointObjA, qPoint const & qPointObjB ) {
////return qPointObjA.getDistance() < qPointObjB.getDistance();
//    return qPointObjA.distance < qPointObjB.distance;
//}

//bool qPoint::operator<( const qPoint *qPointObjB) const {
//    return qPointObjA->getDistance() < qPointObjB->getDistance();
//}

double qPoint::getDistance() const { return distance; }

std::string qPoint::getName() const { return name; }
