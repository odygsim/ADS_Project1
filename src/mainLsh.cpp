// This file will be the main of lsh
#include<cstring>
#include "../inc/KNeighborsClassifier.h"

int main(int argc, char **argv) {
    using namespace std;

    int L1, k1, L = 5, k = 4, w = 5000, numNeighbors = 1, topLimit =
            4 * L, r = 0, dimension = 0, m = 0;// INT32_MAX - 5;
    int id = 0;
    char *pEnd;
    const char *arg;
    string newline = "\n", space = " ", metric_name = "manhattan", stats, result;
    string oFileName, iFileName, qFileName, output;

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
            L1 = (int) strtol(*++argv, &pEnd, 10);
        } else if (!strcmp(arg, "-k"))
            k1 = (int) strtol(*++argv, &pEnd, 10);
        else if (!strcmp(arg, "-q"))
            qFileName = *++argv;
        else if (!strcmp(arg, "-d"))
            iFileName = *++argv;
    }

//    cout << "Running Instance with args " << iFileName << " " << qFileName << " " << k << " " << L << " " << oFileName
//         << endl;
    runLSH<int>(id, iFileName, qFileName, output, L, k, w, numNeighbors, topLimit, m);
//    / Tests for lsh
//    for (int i = 0; i < 4; ++i) { // Run (49280) tests
//        if (i > 1) { m = 0; }   // two with m = 0 and 2 with m = int32max -5
////        else    m = INT32_MAX - 5;
//        for (int l = 4; l < 31; ++l) { // Try L {4,31}
//            topLimit = 3 * l;
//            if (i > 1) topLimit = 4 * l;  // Try topLimit {3,4} * L from 2 tests each one
//            for (int k = 4; k < 11; ++k) { // Try k from 4 until 11
//                for (int w = 1000; w < 12001; w += 500) { // Try w from 1000 until 12000
//                    id++;
//
//                    runLSH<int>(id, iFileName, qFileName,output, l, k, w, numNeighbors, topLimit, m);
//                }
//
//            }
//        }
//    }

    return 0;
}
