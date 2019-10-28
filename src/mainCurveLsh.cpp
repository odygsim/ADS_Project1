//
// Created by aikio on 25/10/19.
//

#include <string>
#include <tuple>
#include "../inc/util.h"
#include "../inc/CurvesLSH.h"
#include "../inc/KNeighborsClassifier.h"

template<typename D>
void runCurveLsh(int id, std::string &iFileName, std::string &qFileName, std::string &oFileName,
                 std::string metric_name = "euclidean", int L_grid = 4, int k = 3, double w = 3000) {
    /**
     * @brief Runs CurvesLsh for cube knn algorithm.
     * @params TODO complete.
     */
    using namespace std;

    typedef list< vector< vector<D> > > CX;
    typedef list<string> CY;
    typedef vector< vector< D > > X;
    typedef string Y;
    typedef double TX;
    typedef ExactKNeighbors2<CX, X, TX, CY, Y> EKNN_;
    typedef chrono::steady_clock::time_point timePoint;
    // these list are returned by predictWithTime method.
    std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> A, E;

    // LSH for curves structure
    typedef double HQ_DX;
    typedef std::string HQ_DY;
    typedef std::vector<double> HQ_IDX;
//    typedef Hypercube< std::vector<double>, double, std::tuple< std::vector< std::vector<double> >, std::string >  > HQStruct;
    typedef LSH< std::vector<double>, double, std::tuple< std::vector< std::vector<double> >, std::string >  > LSHStruct;
//    typedef CurvesLSH< HQ_DX, HQ_DY, Hypercube< std::vector<double>, double, std::tuple< std::vector< std::vector<double> >, std::string >  > > CLSH_HQ_Struct;
    typedef CurvesLSH< HQ_DX, HQ_DY, LSHStruct > CLSH_LSH_Struct;

    timePoint start; // start variable for each method execution.
    timePoint AppStart = initTime(); // start variable for whole program.
    int dimension, k_hi = 4, r = 0;
    double radius = 0;
    string newline = "\n", space = " ", stats, result, output;
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
    //readDataAndLabelsFromFile<CX, CY, X, Y>(iFileName, iDataList, iLabelList);
    readTrajectories<CX, CY, X, TX>(iFileName, iDataList, iLabelList);

    // Get points dimension and min and max number of points in curves dataset
    dimension = iDataList.begin()->begin()->size();
    // Max number of points
    typename CX::iterator max_itr = max_element(iDataList.begin(),iDataList.end(),longestVec<double>);
    int max_curve_sz = max_itr->size();
    // Min number of points
    typename CX::iterator min_itr = max_element(iDataList.begin(),iDataList.end(),shortestVec<double>);
    int min_curve_sz = min_itr->size();
    // Set the max curve size to use
    int maxPointsInCurves = 1.1 * max_curve_sz;
    // Set delta to be used in curves LSH
    double delta = 0.001; //4 * dim * min_curve_sz;
    // Maximum number in coordinates
    double maxCoordNum=55;

    // Initialize algorithms ( Hypercube, HQ_Classifier, ExactKNN, EKNN_Classifier )
    CLSH_LSH_Struct* CLSH = new CLSH_LSH_Struct(dimension, delta, k, maxPointsInCurves, L_grid , w , metric_name, maxCoordNum );

    auto *clLsh = new KNeighborsClassifier<CLSH_LSH_Struct *, CX, X, TX, CY, Y>(CLSH);
    auto *eknn = new EKNN_(radius, metric_name);
    auto *clEknn = new KNeighborsClassifier<EKNN_ *, CX, X, TX, CY, Y>(eknn);

    // KNN train set fitting
    std::cout << " Fitting train set to Exact KNN..." << newline;
    start = initTime();                                         // timestamp start
    clEknn->fit(iDataList, iLabelList);                         // fit exact knn
    timeList.push_back(getElapsed(start));                      // timestamp end

    // Hypercube train set fitting
    std::cout << " Fitting train set to CurvesLSH-LSH ANN..." << newline;
    start = initTime();                                         // timestamp start
    clLsh->fit(iDataList, iLabelList);                         // fit approx knn
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
        readTrajectories<CX, CY, X, TX>(qFileName, qDataList, qLabelList);

        // Ask for output file
        if ( inputFileMessageDialog("Please provide the output file path (or file name if in running directory)",
                                    "No output file has been provided! Program terminates..please try again",
                                    oFileName) != 0 ) { break; }

        // Inform user for running case
        std::cout << "Running CurvesLSH-Lsh with arguments: " << "input file = \"" << iFileName << "\" query file = \"" << qFileName << "\" output file =  \"" << oFileName
                  << " k_vec = " << k << " L_grid = " << L_grid << "\n";

        // KNN query set running
        std::cout << " Running query set in Exact KNN..." << newline;
        start = initTime();                                         // timestamp start
        E = clEknn->predictWithTimeAndDistance(qDataList);          // predict exact knn
        timeList.push_back(getElapsed(start));                      // timestamp end

        // Hypercube query set running
        std::cout << " Running query set in CurvesLSH-LSH ANN..." << newline;
        start = initTime();                                         // timestamp start
        A = clLsh->predictWithTimeAndDistance(qDataList);           // predict Approx knn
        timeList.push_back(getElapsed(start));                      // timestamp end

        /// Calculate results and stats also
        results = unrollResult<Y, TX, CY> (E, A, qLabelList, "CurvesLSH-LSH");       // unroll the results and the stats to tuple strings.
        timeList.push_back(getElapsed(AppStart));                      // timestamp App End

        // Write result, stats to according files
        string fields  = "AlgorithmName,id,datetime,L,w,m,k,topLimitLsh,radius,dimension,Accuracy,Time,iFileName,trainSize,fitExactTime,predictExactTime,fitApproxTime,predictApproxtime,AppTime\n";
        oFile.open("stats_Cube.csv", std::ios::out | std::ios::app);
        if ( ! oFile.is_open() ) {
            break;
        }
        string res;
        res = "CurvesLSH-LSH," + to_string(id) + "," + getDatetime(false) + ", L_grid = " + to_string(L_grid) + ", w = " + to_string(w)
               + ", k_vec = " + to_string(k) + ", dimension = " + to_string(dimension) + "," +
              ", maximumCoordinate used = " + to_string(maxCoordNum) + "," +
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
    delete CLSH;
    delete clLsh;
}

int main(int argc, char **argv) {

    using namespace std;
    // Set program parameters
    int k = 4, M = 10, probes =2, L_grid = 4;
    double w = 0.01;
    int id = 0;

    string newline = "\n", space = " ", metric_name = "euclidean", stats, result;
    string iFileName, qFileName, oFileName;

    // Read/Verify program parameters
    if ( readCurvesLSHWithLshParameters(argc, argv, iFileName, qFileName, oFileName, k, L_grid) != 0  ){
        return -1;
    }

    // Run CurveLSH - Hypercube
    runCurveLsh<double>(id, iFileName, qFileName, oFileName, metric_name, L_grid, k, w);

    return 0;
}

