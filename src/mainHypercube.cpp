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

    // Find size of vector and data
    int d = (*iDataList.begin()).size();
    int dataSize = iDataList.size();

    // Create a hypercube structure
    Hypercube<vector<int>, int, string>* HQ = new Hypercube<vector<int>, int, string>(d,3000,3,10,2,4,0);

    typedef typename list<vector<int>>::iterator tdIt; // Iterator on the list of vectors
    typedef typename list<string>::iterator tyIt; // iterator on the list of strings
    tdIt iteratorData; // Init Iterator on list of vectors
    tdIt queryIterData;
    tdIt itDE = iDataList.end(); // end of data iterator
    tyIt iteratorLabels; // Iterator on the list of strings
    auto data = iDataList;
    auto labels = iLabelList;

    // Add dataset points
    int i=0;
    for (iteratorData = data.begin(), iteratorLabels = labels.begin();
         iteratorData != itDE; ++iteratorData, ++iteratorLabels) {
        HQ->addPoint(*iteratorData, *iteratorLabels); // Add a vector<int> and string
        std::cout << i++ << "\n";
        if (i>10) break;
    }

    // Query for a point to test
    queryIterData = qDataList.begin();
    list< tuple<std::string, int> > resList = HQ->queryPoint( *iteratorData);

    return 0;

}