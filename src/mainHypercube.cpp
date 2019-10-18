// This file will be the main of Hypercude program


#include "../inc/KNeighborsClassifier.h"
#include "../inc/Hypercube.h"

template<typename D>
void
runCube(int id, std::string &iFileName, std::string &qFileName, std::string &outFile, int L = 5, int k = 4, int w = 5000, int probes = 2,
       int numNeighbors = 1, int topLimi = 4, int m = 0) {
    /**
     * @brief Runs cube knn algorithm.
     * @params TODO complete.
     */

    using namespace std;
    typedef list<vector<D>> CX;
    typedef list<string> CY;
    typedef vector<D> X;
    typedef string Y;
    typedef int TX;
    typedef Hypercube<X, TX, Y> HC_;
    typedef ExactKNeighbors<CX, X, TX, CY, Y> EKNN_;
    typedef chrono::steady_clock::time_point timePoint;
    // these list are returned by predictWithTime method.
    std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> A, E;

    timePoint start; // start variable for each method execution.
    timePoint AppStart = initTime(); // start variable for whole program.
    int topLimit = topLimi, dimension, k_hi = 4, r = 0;
    string newline = "\n", space = " ", metric_name = "manhattan", stats, result, oFileName, output;
    tuple<string, string> results;
    // declare train list, test train list, query list, test query list
    CX iDataList, qDataList, testIDataList, testQDataList;
    CY iLabelList, qLabelList, testILabelList, testQLabelList;
    vector<double> timeList;                                    // This vector is used to store time values.
    ofstream oFile;
    // Read Train data and query data.
    readDataAndLabelsFromFile2<CX, CY, X, Y>(iFileName, iDataList, iLabelList);
    readDataAndLabelsFromFile2<CX, CY, X, Y>(qFileName, qDataList, qLabelList, numNeighbors);
    typename CX::iterator iterData1, iterQData; // some iterators
    typename CY::iterator iterLabel1, iterQLabel;

    dimension = iDataList.front().size(); // get dimension of data
    // Initialize algorithms
//    (d,3000,3,10,2,4,0);
//    Hypercube<TID, D, Y>::Hypercube(int d, double w, int k, int maxSearchPoints, int probes, int k_hi, double r):
    HC_ *HCube= new HC_(dimension, w, k, topLimit, probes, k_hi, r);
    auto *clCube = new KNeighborsClassifier<HC_ *, CX, X, TX, CY, Y>(numNeighbors, HCube);
    auto *eknn = new EKNN_(numNeighbors, metric_name);
    auto *clEknn = new KNeighborsClassifier<EKNN_ *, CX, X, TX, CY, Y>(numNeighbors, eknn);

    /// Start fit and predict
    start = initTime();                                         // timestamp start
    clEknn->fit(iDataList, iLabelList);                         // fit exact knn
    timeList.push_back(getElapsed(start));                      // timestamp end
    start = initTime();                                         // timestamp start
    E = clEknn->predictWithTimeAndDistance(qDataList);          // predict exact knn
    timeList.push_back(getElapsed(start));                      // timestamp end
    start = initTime();                                         // timestamp start
    clCube->fit(iDataList, iLabelList);                          // fit approx knn
    timeList.push_back(getElapsed(start));                      // timestamp end
    start = initTime();                                         // timestamp start
    A = clCube->predictWithTimeAndDistance(qDataList);           // predict Approx knn
    timeList.push_back(getElapsed(start));                      // timestamp end
    /// Calculate results and stats also
    results = unrollResult<Y, TX, CY> (E, A, qLabelList, "Hypercube");       // unroll the results and the stats to tuple strings.
    timeList.push_back(getElapsed(AppStart));                      // timestamp App End

    // Write result, stats to according files
    string fields  = "AlgorithmName,id,datetime,L,w,m,k,topLimitLsh,numNeighbors,dimension,Accuracy,Time,iFileName,trainSize,fitExactTime,predictExactTime,fitApproxTime,predictApproxtime,AppTime\n";
    oFile.open("tests/stats_Cube.csv", std::ios::out | std::ios::app);
    string res;
    res = "Cube," + to_string(id) + "," + getDatetime(false) + "," + to_string(L) + "," + to_string(w) + "," +
          to_string(m) + "," + to_string(k) +
          "," + to_string(topLimit) + "," + to_string(numNeighbors) + "," + to_string(dimension) + "," +
          get<1>(results) + "," + getFilename(iFileName) +
          "," + to_string(iDataList.size()) + "," + to_string(timeList[0]) +"," + to_string(timeList[1]) +
          "," + to_string(timeList[2]) +"," + to_string(timeList[3]) +"," + to_string(timeList[4]) +
          "\n";
    oFile << res;
    oFile.close();
    cout << getDatetime(false) << "\t\t\t\t" <<fields;
    cout << getDatetime(false) << "\t\t\t\t" << res;

    oFile.open("tests/Output_Cube__" + getDatetime(true) + ".txt", std::ios::out);
    oFile << get<0>(results);
    oFile << "\n" + get<1>(results) + "\n";
    oFile.close();
//    // Clean Up
    delete eknn;
    delete clEknn;
    delete HCube;
    delete clCube;
}


int main(int argc, char **argv) {
    using namespace std;

    int L = 5, k = 4, w = 1000, numNeighbors = 1, topLimit =
            4 * L, r = 0, dimension = 0,  M = 0, probes = 0, m = 0;// INT32_MAX - 5;
    int id = 0;
    char *pEnd;
    const char *arg;
    string newline = "\n", space = " ", metric_name = "manhattan", stats, result;
    string oFileName, inputFile, queryFile, outputFile;

    // Read/Verify program parameters
    if ( readHypercubeParameters(argc, argv, inputFile, queryFile, outputFile, k, M, probes) != 0 ){
        return -1;
    }

    std::cout << "Running Hypecube with arguments: " << "input file = \"" << inputFile << "\" query file = \"" << queryFile << "\" output file =  \"" << outputFile
              << " k = " << k << " M = " << M  << " probes = " << probes << "\n";

    runCube<int>(id, inputFile, queryFile, outputFile, L, k, w, probes, numNeighbors, topLimit, m);

    return 0;
}


int iimain(int argc, char **argv) {

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
//    readDataAndLabelsFromFile2<CX, CY, X, Y>(iFileName, iDataList, iLabelList);
    readDataAndLabelsFromFile(inputFile, iDataList, iLabelList);

    // Read query file to get dataset in form of label and data lists
//    readDataAndLabelsFromFile2<CX, CY, X, Y>(iFileName, iDataList, iLabelList);
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
//        if (i>10) break;
    }

    // Query for a point to test
    queryIterData = qDataList.begin();
    list< tuple<std::string, int> > resList = HQ->queryPoint( *iteratorData);

    return 0;

}