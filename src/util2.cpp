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

unsigned int trueMod(long a, long b){

    long res;
    res = ( (a % b) + b ) % b;

    return (unsigned int) res;
}

unsigned int calcModOfNumberInPower(long num, int power, long modulator,  std::vector<unsigned int> &modArray){

    long iRes, currRes;
    // Add mod for m^0
    modArray.push_back(1);
    // Calc mod for m^1
    iRes = trueMod(num, modulator);
    // Keep as current
    currRes = iRes;
    // Add mod for m^1
    modArray.push_back(iRes);
    for(int i=1; i<power; i++){
        // Calc and add mod for m^(i+1)
        currRes = trueMod(currRes * iRes, modulator);
        modArray.push_back(currRes);
    }

    return (unsigned int) currRes;
}