// This file will be the main of Hypercude program

#include <stdio.h>
#include <stdlib.h>
#include "../inc/util.h"

int main(int argc, char **argv)
{
    std::string inputFile;
    std::string queryFile;
    std::string outputFile;
    int k, M, probes;

    // Read/Verify program parameters
    if ( readHypercubeParameters(argc, argv, inputFile, queryFile, outputFile, k, M, probes) != 0 ){
        return -1;
    }

    std::cout << "Running Hypecube with arguments: " << "input file = \"" << inputFile << "\" query file = \"" << queryFile << "\" output file =  \"" << outputFile
         << " k = " << k << " M = " << M  << " probes = " << probes << "\n";

    return 0;

}