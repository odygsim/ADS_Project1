//
// Created by aikio on 15/10/19.
//

#include "../inc/util2.h"

int get_int_bit_value(int num, int position){

    int bitVal;
    // Getting the value at certain bit position by bitwise shift
    bitVal =  (num >> position) & 1;
    return bitVal;

}

int set_int_bit_value(int num, int pos, int bitValue){

    // Code idea got from https://www.geeksforgeeks.org/modify-bit-given-position/
    int mask = 1 << pos;
    // Setting the bit value in pos position to bitValue
    int newNum =  (num & ~mask) | ((bitValue << pos) & mask);
    return newNum;
}

int readDataAndLabelsFromFile(std::string rFile, std::list<std::vector<int>>  &fDataList, std::list<std::string> &fLabelList){

    std::vector<std::tuple<std::string, std::vector<int>>> fDataVec;
    // Read data to a string labels, vector structure
    fDataVec = readData2<std::string, std::vector<int>>(rFile);
    // Tranfer data to label and data list
    for (int i = 0; i < fDataVec.size(); ++i) {
        fDataList.push_back(std::get<1>(fDataVec[i]));
        fLabelList.push_back(std::get<0>(fDataVec[i]));
    }

    return 0;
}