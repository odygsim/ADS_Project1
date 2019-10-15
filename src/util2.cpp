//
// Created by aikio on 15/10/19.
//

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