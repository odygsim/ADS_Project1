//
// Created by user on 10/12/19.
//

#include "../inc/Point.h"

#include <utility>

std::vector<int> Point::getList() {
    return this->dList;
}
Point::Point(std::string n, std::vector<int> l) {
    name=std::move(n);
    dList = std::move(l);
}