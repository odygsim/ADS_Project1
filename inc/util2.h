//
// Created by aikio on 15/10/19.
//

#ifndef ADS_PROJECT1_UTIL2_H
#define ADS_PROJECT1_UTIL2_H

#include <utility>
#include <string>
#include <list>
#include <iostream>
#include <vector>
#include "util.h"

int get_int_bit_value(int num, int bitPosition);

int set_int_bit_value(int num, int pos, int bitValue);

int readDataAndLabelsFromFile(std::string rFile, std::list<std::vector<int>>  &iDataList, std::list<std::string> &iLabelList);

#endif //ADS_PROJECT1_UTIL2_H
