// This file will be the main of Hypercude program


#include "../inc/KNeighborsClassifier.h"
#include "../inc/Hypercube.h"

template<typename D>
void runCube(int id, std::string &iFileName, std::string &qFileName, std::string &oFileName, int M = 5, int k = 3, int w = 3000, int probes = 2,
        int topLimi = 10, int m = 0) {
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
    double radius = 0;
    string newline = "\n", space = " ", metric_name = "manhattan", stats, result, output;
    tuple<string, string> results;

    // Declare train list, test train list, query list, test query list
    CX iDataList, qDataList, testIDataList, testQDataList;
    CY iLabelList, qLabelList, testILabelList, testQLabelList;
    vector<double> timeList;  // This vector is used to store time values.
    ofstream oFile;

    // Iterators for data
    typename CX::iterator iterData1, iterQData;
    typename CY::iterator iterLabel1, iterQLabel;

    // Ask for input file if not given in arguments
    if ( inputFileMessageDialog("Please provide the training dataset file path (or file name if in running directory)",
                                "No input file has been provided! Program terminates..please try again",
                                iFileName) != 0 ) { exit(EXIT_FAILURE); }

    // Read Train data
    readDataAndLabelsFromFile<CX, CY, X, Y>(iFileName, iDataList, iLabelList);

    // Get dimension of data
    dimension = iDataList.front().size();

    // Initialize algorithms ( Hypercube, HQ_Classifier, ExactKNN, EKNN_Classifier )
    HC_ *HCube= new HC_(dimension, w, k, M, probes, k_hi, r);
    auto *clCube = new KNeighborsClassifier<HC_ *, CX, X, TX, CY, Y>(HCube);
    auto *eknn = new EKNN_(radius, metric_name);
    auto *clEknn = new KNeighborsClassifier<EKNN_ *, CX, X, TX, CY, Y>(eknn);

    // KNN train set fitting
    std::cout << " Fitting train set to Exact KNN..." << newline;
    start = initTime();                                         // timestamp start
    clEknn->fit(iDataList, iLabelList);                         // fit exact knn
    timeList.push_back(getElapsed(start));                      // timestamp end

    // Hypercube train set fitting
    std::cout << " Fitting train set to Hypercube ANN..." << newline;
    start = initTime();                                         // timestamp start
    clCube->fit(iDataList, iLabelList);                         // fit approx knn
    timeList.push_back(getElapsed(start));                      // timestamp end

    // User's answer on repeated procedure (querying with new file)
    std::string userAnswer = "Y";

    // Repeat until user terminates
    while ( userAnswer == "Y" || userAnswer == "y") {

        // Ask for query file
        if ( inputFileMessageDialog("Please provide the query dataset file path (or file name if in running directory)",
                                    "No query file has been provided! Program terminates..please try again",
                                    qFileName) != 0 ) { break; }

        // Read Query data
        std::cout << "Reading querying data..." << '\n';
        if ( !readDataAndLabelsFromFile<CX, CY, X, Y>(qFileName, qDataList, qLabelList, radius) ) { break; };

        // Ask for output file
        if ( inputFileMessageDialog("Please provide the output file path (or file name if in running directory)",
                                    "No output file has been provided! Program terminates..please try again",
                                    oFileName) != 0 ) { break; }

        // Inform user for running case
        std::cout << "Running Hypecube with arguments: " << "input file = \"" << iFileName << "\" query file = \"" << qFileName << "\" output file =  \"" << oFileName
                  << " k = " << k << " M = " << M  << " probes = " << probes << "\n";

        // KNN query set running
        std::cout << " Running query set in Exact KNN..." << newline;
        start = initTime();                                         // timestamp start
        E = clEknn->predictWithTimeAndDistance(qDataList);          // predict exact knn
        timeList.push_back(getElapsed(start));                      // timestamp end

        // Hypercube query set running
        std::cout << " Running query set in Hypercube ANN..." << newline;
        start = initTime();                                         // timestamp start
        A = clCube->predictWithTimeAndDistance(qDataList);           // predict Approx knn
        timeList.push_back(getElapsed(start));                      // timestamp end

        /// Calculate results and stats also
        results = unrollResult<Y, TX, CY> (E, A, qLabelList, "Hypercube");       // unroll the results and the stats to tuple strings.
        timeList.push_back(getElapsed(AppStart));                      // timestamp App End

        // Write result, stats to according files
        string fields  = "AlgorithmName,id,datetime,L,w,m,k,topLimitLsh,radius,dimension,Accuracy,Time,iFileName,trainSize,fitExactTime,predictExactTime,fitApproxTime,predictApproxtime,AppTime\n";
        oFile.open("stats_Cube.csv", std::ios::out | std::ios::app);
        if ( ! oFile.is_open() ) {
            break;
        }
        string res;
        res = "Cube," + to_string(id) + "," + getDatetime(false) + "," + to_string(M) + "," + to_string(w) + "," +
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

        oFile.open(oFileName + "_" + getDatetime(true) + ".txt", std::ios::out);
        oFile << get<0>(results);
        oFile << "\n" + get<1>(results) + "\n";
        oFile.close();

        // Ask user for repeating
        std::cout << "Would you like to continue with a new query file? (Y/N)" << '\n';
        std::cin >> userAnswer ;
        if ( userAnswer == "N" || userAnswer == "n") { break; }

        // Clear files to repeat
        qFileName = "";
        oFileName = "";

        // Clear query structures to start again
        qDataList.clear();
        qLabelList.clear();

    }

    // Clean Up
    delete eknn;
    delete clEknn;
    delete HCube;
    delete clCube;
}


int main(int argc, char **argv) {
    using namespace std;

    int id=0, L = 5, k = 4, w = 1000,  topLimit =
            4 * L, M = 0, probes = 0, m = 0;// INT32_MAX - 5;
    string newline = "\n", space = " ", metric_name = "manhattan", stats, result;
    string inputFile, queryFile, outputFile;

    // Read/Verify program parameters
    if ( readHypercubeParameters(argc, argv, inputFile, queryFile, outputFile, k, M, probes) != 0 ){
        return -1;
    }

    // Run Hypercube
    runCube<int>(id, inputFile, queryFile, outputFile, M, k, w, probes, topLimit, m);

    return 0;
}


//int iimain(int argc, char **argv) {
//
//    using namespace std;
//
//    std::string inputFile;
//    std::string queryFile;
//    std::string outputFile;
//    int k, M, probes;
//
//    list<vector<int>> iDataList;
//    list<string> iLabelList;
//    list<vector<int>> qDataList;
//    list<string> qLabelList;
//
//
//    // Read/Verify program parameters
//    if ( readHypercubeParameters(argc, argv, inputFile, queryFile, outputFile, k, M, probes) != 0 ){
//        return -1;
//    }
//
//
//    std::cout << "Running Hypecube with arguments: " << "input file = \"" << inputFile << "\" query file = \"" << queryFile << "\" output file =  \"" << outputFile
//         << " k = " << k << " M = " << M  << " probes = " << probes << "\n";
//
//    // Read input file to get dataset in form of label and data lists
////    readDataAndLabelsFromFile2<CX, CY, X, Y>(iFileName, iDataList, iLabelList);
//    readDataAndLabelsFromFile(inputFile, iDataList, iLabelList);
//
//    // Read query file to get dataset in form of label and data lists
////    readDataAndLabelsFromFile2<CX, CY, X, Y>(iFileName, iDataList, iLabelList);
//    readDataAndLabelsFromFile(queryFile, qDataList, qLabelList);
//
//    // Find size of vector and data
//    int d = (*iDataList.begin()).size();
//    int dataSize = iDataList.size();
//
//    // Create a hypercube structure
//    Hypercube<vector<int>, int, string>* HQ = new Hypercube<vector<int>, int, string>(d,3000,3,10,2,4,0);
//
//    typedef typename list<vector<int>>::iterator tdIt; // Iterator on the list of vectors
//    typedef typename list<string>::iterator tyIt; // iterator on the list of strings
//    tdIt iteratorData; // Init Iterator on list of vectors
//    tdIt queryIterData;
//    tdIt itDE = iDataList.end(); // end of data iterator
//    tyIt iteratorLabels; // Iterator on the list of strings
//    auto data = iDataList;
//    auto labels = iLabelList;
//
//    // Add dataset points
//    int i=0;
//    for (iteratorData = data.begin(), iteratorLabels = labels.begin();
//         iteratorData != itDE; ++iteratorData, ++iteratorLabels) {
//        HQ->addX(*iteratorData, *iteratorLabels); // Add a vector<int> and string
//        std::cout << i++ << "\n";
////        if (i>10) break;
//    }
//
//    // Query for a point to test
//    queryIterData = qDataList.begin();
//    list< tuple<std::string, int> > resList = HQ->queryX(*iteratorData);
//
//    return 0;
//
//}