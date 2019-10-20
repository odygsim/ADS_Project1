// This file will be the main of lsh
#include "../inc/KNeighborsClassifier.h"
//#include<future>
//#include <thread>
template<typename D>
void
runLSH(int id, std::string &iFileName, std::string &qFileName, std::string &outFile, int L = 5, int k = 4, int w = 5000,
        int topLimi = 4, int m = 0) {
    /**
     * @brief Runs lsh knn algorithm.
     * @params TODO complete.
     */

    using namespace std;
    typedef list<vector<D>> CX;
    typedef list<string> CY;
    typedef vector<D> X;
    typedef string Y;
    typedef int TX;
    typedef LSH<X, TX, Y> LSH_;
    typedef ExactKNeighbors<CX, X, TX, CY, Y> EKNN_;
    typedef chrono::steady_clock::time_point timePoint;
    // these list are returned by predictWithTime method.
    std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> A, E;

    timePoint start; // start variable for each method execution.
    timePoint AppStart = initTime(); // start variable for whole program.
    int topLimit = topLimi, dimension;
    double radius = 0;
    string newline = "\n", space = " ", metric_name = "manhattan", stats, result, oFileName, output;
    tuple<string, string> results;
    // declare train list, test train list, query list, test query list
    CX iDataList, qDataList, testIDataList, testQDataList;
    CY iLabelList, qLabelList, testILabelList, testQLabelList;
    vector<double> timeList;                                    // This vector is used to store time values.
    ofstream oFile;
    // Read Train data and query data.
    readDataAndLabelsFromFile2<CX, CY, X, Y>(iFileName, iDataList, iLabelList);
    readDataAndLabelsFromFile2<CX, CY, X, Y>(qFileName, qDataList, qLabelList, radius);
    typename CX::iterator iterData1, iterQData; // some iterators
    typename CY::iterator iterLabel1, iterQLabel;

    dimension = iDataList.front().size(); // get dimension of data
    // Initialize algorithms
    LSH_ *lsh = new LSH<X, TX, Y>(dimension, w, k, L, m, radius, topLimit, metric_name);
    auto *clLsh = new KNeighborsClassifier<LSH_ *, CX, X, TX, CY, Y>(lsh);
    auto *eknn = new EKNN_(radius, metric_name);
    auto *clEknn = new KNeighborsClassifier<EKNN_ *, CX, X, TX, CY, Y>(eknn);

    if (iDataList.size() == 1000000){
        // if big dataset Prefetch results from file.
        string f = "tests/sample_datasets/siftbig/ENN_results.txt";
        E = getENNData<D,Y>(f);
        double ennfit  = 0.688875, ennPredict = 48334.336797;
        timeList.push_back(ennfit);
        timeList.push_back(ennPredict);
    }else{
        // if small dataset run it.
        start = initTime();                                         // timestamp start
        clEknn->fit(iDataList, iLabelList);                         // fit exact knn
        timeList.push_back(getElapsed(start));                      // timestamp end
        start = initTime();                                         // timestamp start
        E = clEknn->predictWithTimeAndDistance(qDataList);          // predict exact knn
        timeList.push_back(getElapsed(start));                      // timestamp end
    }
    /// Start fit and predict
    start = initTime();                                         // timestamp start
    clLsh->fit(iDataList, iLabelList);                          // fit approx knn
    timeList.push_back(getElapsed(start));                      // timestamp end
    start = initTime();                                         // timestamp start
    A = clLsh->predictWithTimeAndDistance(qDataList);           // predict Approx knn
    timeList.push_back(getElapsed(start));                      // timestamp end
    /// Calculate results and stats also
    results = unrollResult<Y, TX, CY> (E, A, qLabelList);       // unroll the results and the stats to tuple strings.
    timeList.push_back(getElapsed(AppStart));                      // timestamp App End

    // Write result, stats to according files
    string fields  = "AlgorithmName,id,datetime,L,w,m,k,topLimitLsh,radius,dimension,maxADistance,meanATime,AccuracyA,iFileName,trainSize,fitExactTime,predictExactTime,fitApproxTime,predictApproxtime,AppTime\n";
    oFile.open("tests/lsh_results/test3/stats2.csv", std::ios::out | std::ios::app);
    string res;
    res = "LSH," + to_string(id) + "," + getDatetime(false) + "," + to_string(L) + "," + to_string(w) + "," +
          to_string(m) + "," + to_string(k) +
          "," + to_string(topLimit) + "," + to_string(radius) + "," + to_string(dimension) + "," +
          get<1>(results) + "," + getFilename(iFileName) +
          "," + to_string(iDataList.size()) + "," + to_string(timeList[0]) + "," + to_string(timeList[1]) +
          "," + to_string(timeList[2]) + "," + to_string(timeList[3]) + "," + to_string(timeList[4]) +
          "\n";
    oFile << res;
    oFile.close();
    cout << getDatetime(false) << "\t\t\t\t" <<fields;
    cout << getDatetime(false) << "\t\t\t\t" << res;

    oFile.open("tests/lsh_results/test3/out/Output__" + getDatetime(true) + ".txt", std::ios::out);
    oFile << get<0>(results);
    oFile << "\n" + get<1>(results) + "\n";
    oFile.close();
//    // Clean Up
    delete eknn;
    delete clEknn;
    delete lsh;
    delete clLsh;
}


int main(int argc, char **argv) {

    using namespace std;

    int L = 5, k = 4, w = 3000, topLimit =
            4 * L, m =  INT32_MAX - 5; int id = 0;
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
    runLSH<int>(id, iFileName, qFileName, output, L, k, w, topLimit, m);

    return 0;
}

