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

int readCurvesLSHWithHypercubeParameters(int argc, char **argv, std::string &inputFile, std::string &queryFile,
                                         std::string &outputFile, int &k_hypercube, int &M, int &probes, int &L_grid) {
    extern char *optarg;
    int opt = 0;

    //Set default parameters values
    k_hypercube = 3;
    M = 10;
    probes = 2;
    L_grid = 4;

    //Specify expected options
    static struct option grid_cube_options[] = {
            {"d",      required_argument, 0, 'd'},
            {"q",      required_argument, 0, 'q'},
            {"o",      required_argument, 0, 'o'},
            {"k_hypercube",      required_argument, 0, 'k'},
            {"M",      required_argument, 0, 'M'},
            {"probes", required_argument, 0, 'p'},
            {"L_grid", required_argument, 0, 'l'}
    };

    int longIndex = 0;
    int dflag = 0, qflag = 0, oflag = 0;

    // Get options given
    while ((opt = getopt_long_only(argc, argv, "",
                                   grid_cube_options, &longIndex)) != -1) {
        switch (opt) {
            case 'd' :
                dflag = 1;
                inputFile = optarg;
                break;
            case 'q' :
                qflag = 1;
                queryFile = optarg;
                break;
            case 'o' :
                oflag = 1;
                outputFile = optarg;
                break;
            case 'k' :
                k_hypercube = atoi(optarg);
                break;
            case 'M' :
                M = atoi(optarg);
                break;
            case 'p' :
                probes = atoi(optarg);
                break;
            case 'l' :
                L_grid = atoi(optarg);
                break;
            default:
                print_curvesLSHWithHypercube_usage();
                exit(EXIT_FAILURE);
        }
    }

    //  Check if necessary options have been provided
    if (dflag == 0) {
        fprintf(stderr, "cube: missing -d option\n");
        print_cube_usage();
        exit(EXIT_FAILURE);
    } else if (qflag == 0) {
        fprintf(stderr, "cube: missing -q option\n");
        print_cube_usage();
        exit(EXIT_FAILURE);
    } else if (oflag == 0) {
        fprintf(stderr, "cube: missing -o option\n");
        print_cube_usage();
        exit(EXIT_FAILURE);
    }


    return 0;
}

void print_curvesLSHWithHypercube_usage() {
    fprintf(stderr, "Usage: curve_grid_hypecube -d <input file> -q <query file> -k_hypecube <int> -M <int> -probes <int> L_grid <int> -o <output file>\n");
}
