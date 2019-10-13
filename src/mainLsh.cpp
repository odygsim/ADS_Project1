// This file will be the main of lsh

//#include<iostream>
#include <chrono>
#include<cstring>
//#include<cstdlib>
//#include "../inc/Point.h"
#include "../inc/util.h"

//template<class  X, class Y>
int main(int argc, char **argv) {
    using namespace std;

    int L = 0, k = 0, r = 0;
    char *pEnd;
    const char *arg;
    string newline = "\n";
    string space = " ";
    list<const qPoint *> resultList;
    const qPoint *result;
    string oFileName, iFileName, qFileName, output;
//    vector<const Point *> iDataList;
//    vector<const Point *> qDataList;
    vector<std::tuple<string, vector<int>>>  iDataList;
    vector<std::tuple<string, vector<int>>>  qDataList;
//    vector<const Point *> qDataList;
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
//    vector<std::tuple<string, vector<int>>> iDataList(readData2<string, vector<int>>(iFileName));
    iDataList = readData2<std::string, std::vector<int>>(qFileName);
    qDataList = readData2<std::string, std::vector<int>>(qFileName);

//    r=meanDistanceBetweenPoints(iDataList);
    // Count CPU+WALL Time https://stackoverflow.com/questions/2808398/easily-measure-elapsed-time
//    for (auto & i : qDataList) {
//        cout << i->getName() << '|';
//    }
//    cout << endl;
//    for (auto & b : iDataList) {
//        cout << h.calculatePoint(b) << '|';
//    }
    cout << endl;
    exit(1);
//    list< const qPoint *> ::iterator itS;
//    list< const qPoint *> ::iterator itE;
//    list< const qPoint *> ::iterator it;// = resultList.begin();
//    for (auto &i : qDataList) {
////        char str[20];
//        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//        resultList = exactKNN(iDataList, i, 1);
//        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//        itS = resultList.begin();
//        itE = resultList.end();
//        for (it = itS; it != itE; ++it) {
//            result = *it;
//            output += "Query: " + i->getName() + space;
//            output += "Nearest neighbor: " + result->getName() + space;
//            output += "distanceLSH: " + space;
//            output += "distanceTrue: " + std::to_string(result->getDistance()) + space;
////            sprintf(str, "%.2f", result->getDistance());
////            string a(str, strlen(str));
////            output += "distanceTrue: " ;
////            output += a + space;// std::to_string(result->getDistance()) + "\n";
//            output += "tLSH: " + space;
//            output += "tTrue: " + std::to_string(
//                    (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0) + space;
//            cout << output << endl;
//            output = "";
//        }
////        break;
//    }
//    // Clean Up
//
//    for (vector<const Point *>::iterator it = iDataList.begin(); it != iDataList.end(); ++it) { delete *it; }
//    for (vector<const Point *>::iterator it = qDataList.begin(); it != qDataList.end(); ++it) { delete *it; }
    return 0;
}
