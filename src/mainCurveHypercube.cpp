//
// Created by ody on 17/10/19.
//

#include<cstring>
#include "../inc/KNeighborsClassifier.h"

void runCurveGridHypercube(int id, std::string &iFileName, std::string &qFileName, std::string &outFile, int L = 5,
                           int k = 4, int w = 5000, int numNeighbors = 1, int topLimi = 4, int m = 0, int probes = 0,
                           int M = 0) {
    /**
     * @brief Runs lsh knn algorithm.
     * @params TODO complete.
     */

    using namespace std;
    typedef vector<tuple<double, double>> X;
    typedef list<X> CX;
    typedef list<string> CY;
    typedef string Y;
//    typedef double TX;
//    typedef LSH<X, TX, Y> LSH_;
//    typedef ExactKNeighbors<CX, X, TX, CY, Y> EKNN_;
    typedef chrono::steady_clock::time_point timePoint;
    // these list are returned by predictWithTime method.
//    std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> A, E;

    timePoint start; // start variable for each method execution.
    timePoint AppStart = initTime(); // start variable for whole program.
    int topLimit = topLimi, dimension;
    string newline = "\n", space = " ", metric_name = "manhattan", stats, result;
    tuple<string, string> results;
    // declare train list, test train list, query list, test query list
    CX iDataList, qDataList, testIDataList, testQDataList;
    CY iLabelList, qLabelList, testILabelList, testQLabelList;
    vector<double> timeList;                                    // This vector is used to store time values.
    string oFileName, output;
    ofstream oFile;
    // Read Train data and query data.
    start = initTime();                                         // timestamp start
    readTrajectories<CX, CY, X, Y>(iFileName, iDataList, iLabelList);
    readTrajectories<CX, CY, X, Y>(qFileName, qDataList, qLabelList);
    cout << "Time to read files : " << getElapsed(start) << " list Sizes " << iDataList.size() << " " << iLabelList.size() << " " << qDataList.size()<< endl;
    exit(1);
    typename CX::iterator iterData1; // some iterators
    typename CY::iterator iterLabel1;
    typename CX::iterator iterQData;
    typename CY::iterator iterQLabel;

    dimension = iDataList.front().size(); // get dimension of data
    // Initialize algorithms
//    LSH_ *lsh = new LSH<X, TX, Y>(w, dimension, k, L, m, numNeighbors, topLimit, metric_name);
//    auto *clLsh = new KNeighborsClassifier<LSH_ *, CX, X, TX, CY, Y>(numNeighbors, lsh);
//    auto *eknn = new EKNN_(numNeighbors, metric_name);
//    auto *clEknn = new KNeighborsClassifier<EKNN_ *, CX, X, TX, CY, Y>(numNeighbors, eknn);
    /// Uncomment next 2 lines if you want to get a w based on r. Find r, for small input takes ~5min, and r ~= 1200
//    r = meanDisVtanceBetweenPoints<list<vector<int>>, vector<int>, int>(iDataList);
//    w = 4*r;

    /// Start Test Memory leaks with valgrind
//    int i = 0;
//    for (i = 0, iterData1 = iDataList.begin(), iterLabel1 = iLabelList.begin(), iterQData = qDataList.begin(), iterQLabel = qLabelList.begin() ;
//    i < 10; i++, iterData1++, iterLabel1++, iterQData++, iterQLabel++){
//        testIDataList.push_back(*iterData1);
//        testILabelList.push_back(*iterLabel1);
//        testQDataList.push_back(*iterQData);
//        testQLabelList.push_back(*iterQLabel);
//    }
//    clEknn->fit(testIDataList, testILabelList);
//    clEknn->predictWithTimeAndDistance(testQDataList);
//    clLsh->fit(testIDataList, testILabelList);
//    clLsh->predictWithTimeAndDistance(testQDataList);
    /// End of Test

    /// Start fit and predict
//    start = initTime();                                         // timestamp start
//    clEknn->fit(iDataList, iLabelList);                         // fit exact knn
//    timeList.push_back(getElapsed(start));                      // timestamp end
//    start = initTime();                                         // timestamp start
//    E = clEknn->predictWithTimeAndDistance(qDataList);          // predict exact knn
////    exit(1);
//    timeList.push_back(getElapsed(start));                      // timestamp end
//    start = initTime();                                         // timestamp start
//    clLsh->fit(iDataList, iLabelList);                          // fit approx knn
//    timeList.push_back(getElapsed(start));                      // timestamp end
//    start = initTime();                                         // timestamp start
//    A = clLsh->predictWithTimeAndDistance(qDataList);           // predict Approx knn
//    timeList.push_back(getElapsed(start));                      // timestamp end
//    /// Calculate results and stats also
//    results = unrollResult<Y, TX, CY> (E, A, qLabelList);       // unroll the results and the stats to tuple strings.
//    timeList.push_back(getElapsed(AppStart));                      // timestamp App End
//
//    // Write result, stats to according files
//    string fields  = "AlgorithmName,id,datetime,L,w,m,k,topLimitLsh,numNeighbors,dimension,Accuracy,Time,iFileName,trainSize,fitExactTime,predictExactTime,fitApproxTime,predictApproxtime,AppTime\n";
//    oFile.open("tests/lsh_results/test3/stats.csv", std::ios::out | std::ios::app);
//    string res;
//    res = "LSH," + to_string(id) + "," + getDatetime(false) + "," + to_string(L) + "," + to_string(w) + "," +
//          to_string(m) + "," + to_string(k) +
//          "," + to_string(topLimit) + "," + to_string(numNeighbors) + "," + to_string(dimension) + "," +
//          get<1>(results) + "," + getFilename(iFileName) +
//          "," + to_string(iDataList.size()) + "," + to_string(timeList[0]) +"," + to_string(timeList[1]) +
//          "," + to_string(timeList[2]) +"," + to_string(timeList[3]) +"," + to_string(timeList[4]) +
//          "\n";
//    oFile << res;
//    oFile.close();
//    cout << getDatetime(false) << "\t\t\t\t" <<fields;
//    cout << getDatetime(false) << "\t\t\t\t" << res;
//
//    oFile.open("tests/lsh_results/test3/out/Output__" + getDatetime(true) + ".txt", std::ios::out);
//    oFile << get<0>(results);
//    oFile << "\n" + get<1>(results) + "\n";
//    oFile.close();
////    // Clean Up
////
//    delete eknn;
//    delete clEknn;
//    delete lsh;
//    delete clLsh;
}

int imain(int argc, char **argv) {
    using namespace std;

    int L1, k1, L = 5, k = 4, w = 5000, numNeighbors = 1, topLimit =
            4 * L, probes, r = 0, dimension = 0, M = 0, m = 0;// INT32_MAX - 5;
    int id = 0;
    char *pEnd;
    const char *arg;
    string newline = "\n", space = " ", metric_name = "manhattan", stats, result;
    string oFileName, iFileName, qFileName, output;

    //Read args
    if (argc != 13) {
        fprintf(stderr, "Usage : %s -d <input file> -q <query file> -k <int> -L <int> -o <output file>\n", argv[0]);
        return 1;
    }
    while (--argc && argc > 6) {
        arg = *++argv;
        if (arg == NULL) break;
        if (!strcmp(arg, "-o")) {
            if (argc > 1 && --argc)
                oFileName = *++argv;
        } else if (!strcmp(arg, "-L_grid"))
            L = (int) strtol(*++argv, &pEnd, 10);
        else if (!strcmp(arg, "-M"))
            M = (int) strtol(*++argv, &pEnd, 10);
        else if (!strcmp(arg, "-k_hypercube"))
            k = (int) strtol(*++argv, &pEnd, 10);
        else if (!strcmp(arg, "-probes"))
            probes = (int) strtol(*++argv, &pEnd, 10);
        else if (!strcmp(arg, "-q"))
            qFileName = *++argv;
        else if (!strcmp(arg, "-d"))
            iFileName = *++argv;
    }

    runCurveGridHypercube(0, iFileName, qFileName, output, L, k, w, numNeighbors, topLimit, m, M);


    return 0;
}
