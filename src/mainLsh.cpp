// This file will be the main of lsh

#include<iostream>
#include <chrono>
#include <getopt.h>
#include<cstring>
#include <fstream>
//#include <stdio.h>
//#include <unistd.h>
//#include <vector>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator
//#include <cstring>
#include<cstdlib>
#include <sstream>
#include "../inc/Point.h"
#include "../inc/util.h"


int main(int argc, char **argv) {
    using namespace std;

    int L = 0, k = 0;
    char *pEnd;
    const char *arg;
    qPoint *result;
    string oFileName, iFileName, qFileName, output;
    vector<const Point *> iDataList;
    vector<const Point *> qDataList;
    //Read args
    if (argc != 11) {
        fprintf(stderr, "Usage : %s -d <input file> -q <query file> -k <int> -L <int> -o <output file>\n", argv[0]);
        return 1;
    }
    while (--argc && argc > 4) {
        arg = *++argv;
        if (!strcmp(arg, "-o")) {
            if (argc > 1 && --argc)
                oFileName = *++argv;
        } else if (!strcmp(arg, "-L")) {
            L = (int) strtol(*++argv, &pEnd, 10);
        } else if (!strcmp(arg, "-k"))
            k = (int) strtol(*++argv, &pEnd, 10);
        else if (!strcmp(arg, "-q"))
            qFileName = *++argv;
        else if (!strcmp(arg, "-d"))
            iFileName = *++argv;
    }

    cout << "Running Instance with args " << iFileName << " " << qFileName << " " << k << " " << L << " " << oFileName
         << endl;
    // Iterate over query file and store each Point's dimension data in a vector
    iDataList = copyData(iFileName);
    qDataList = copyData(qFileName);

    // Try Query Exact KNN
    // Count CPU+WALL Time https://stackoverflow.com/questions/2808398/easily-measure-elapsed-time
//    clock_t t_start, t_end;
//    for (auto & i : qDataList) {
//        cout << i->getName() << '|';
//    }
//    cout << endl;
//    for (auto & b : iDataList) {
//        cout << b->getName() << '|';
//    }
//    cout << endl;
//    exit(1);
    for (auto &i : qDataList) {
        char str[20];
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        result = exactKNN(iDataList, i);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        output += "Query: " + i->getName() + "\n";
        output += "Nearest neighbor: " + result->getName() + "\n";
        output += "distanceLSH: \n";
//        output += "distanceTrue: " + std::to_string(result->getDistance()) + "\n";
        sprintf(str, "%.2f", result->getDistance());
        string a(str,strlen(str));
        output += "distanceTrue: " + a + "\n";// std::to_string(result->getDistance()) + "\n";
        output += "tLSH: \n";
        output += "tTrue: " + std::to_string(
                (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0) + "\n";
        cout << output << endl;
        output = "";
//        break;
    }
    // Clean Up

    for (vector<const Point *>::iterator it = iDataList.begin(); it != iDataList.end(); ++it) { delete *it; }
    for (vector<const Point *>::iterator it = qDataList.begin(); it != qDataList.end(); ++it) { delete *it; }
    return 0;
}
