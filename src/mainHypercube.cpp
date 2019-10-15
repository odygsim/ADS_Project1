// This file will be the main of Hypercude program

#include <stdio.h>
#include <stdlib.h>

#include <list>
#include <string>
#include <vector>

#include "../inc/util.h"
#include "../inc/util2.h"
#include "../inc/Hypercube.h"

int main(int argc, char **argv) {

    using namespace std;

    std::string inputFile;
    std::string queryFile;
    std::string outputFile;
    int k, M, probes;

    list<vector<int>> iDataList;
    list<string> iLabelList;
    list<vector<int>> qDataList;
    list<string> qLabelList;


    // Read/Verify program parameters
    if ( readHypercubeParameters(argc, argv, inputFile, queryFile, outputFile, k, M, probes) != 0 ){
        return -1;
    }

    std::cout << "Running Hypecube with arguments: " << "input file = \"" << inputFile << "\" query file = \"" << queryFile << "\" output file =  \"" << outputFile
         << " k = " << k << " M = " << M  << " probes = " << probes << "\n";

    // Read input file to get dataset in form of label and data lists
    readDataAndLabelsFromFile(inputFile, iDataList, iLabelList);

    // Read query file to get dataset in form of label and data lists
    readDataAndLabelsFromFile(queryFile, qDataList, qLabelList);

    // Create a hypercube structure
    Hypercube<vector<int>, int, string>* HQ = new Hypercube<vector<int>, int, string>(3000,49,3,10,2,4,0);

    return 0;

}