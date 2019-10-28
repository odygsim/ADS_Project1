//
// Created by aikio on 25/10/19.
//

#include <string>
#include <tuple>
#include "../inc/util.h"
#include "../inc/CurvesLSH.h"
#include "../inc/KNeighborsClassifier.h"

template<typename D>
void runCurveCube(int id, std::string &iFileName, std::string &qFileName, std::string &oFileName, std::string metric_name="euclidean",
                  int L_grid = 4, int k =3, int M =10, double w=3000, int probes = 2) {
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
    typedef Hypercube< std::vector<double>, double, std::tuple< std::vector< std::vector<double> >, std::string >  > HQStruct;
    typedef LSH< std::vector<double>, double, std::tuple< std::vector< std::vector<double> >, std::string >  > LSHStruct;
    typedef CurvesLSH< HQ_DX, HQ_DY, Hypercube< std::vector<double>, double, std::tuple< std::vector< std::vector<double> >, std::string >  > > CLSH_HQ_Struct;
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
    CLSH_HQ_Struct* CLSH = new CLSH_HQ_Struct(dimension, delta, k, M, probes, maxPointsInCurves, L_grid , w , metric_name, maxCoordNum );

    auto *clCube = new KNeighborsClassifier<CLSH_HQ_Struct *, CX, X, TX, CY, Y>(CLSH);
    auto *eknn = new EKNN_(radius, metric_name);
    auto *clEknn = new KNeighborsClassifier<EKNN_ *, CX, X, TX, CY, Y>(eknn);

    // KNN train set fitting
    std::cout << " Fitting train set to Exact KNN..." << newline;
    start = initTime();                                         // timestamp start
    clEknn->fit(iDataList, iLabelList);                         // fit exact knn
    timeList.push_back(getElapsed(start));                      // timestamp end

    // Hypercube train set fitting
    std::cout << " Fitting train set to CurvesLSH-Hypercube ANN..." << newline;
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
        readTrajectories<CX, CY, X, TX>(qFileName, qDataList, qLabelList);

        // Ask for output file
        if ( inputFileMessageDialog("Please provide the output file path (or file name if in running directory)",
                                    "No output file has been provided! Program terminates..please try again",
                                    oFileName) != 0 ) { break; }

        // Inform user for running case
        std::cout << "Running CurvesLSH-Hypercube with arguments: " << "input file = \"" << iFileName << "\" query file = \"" << qFileName << "\" output file =  \"" << oFileName
                  << " k = " << k << " M = " << M  << " probes = " << probes << " L_grid = " << L_grid << "\n";

        // KNN query set running
        std::cout << " Running query set in Exact KNN..." << newline;
        start = initTime();                                         // timestamp start
        E = clEknn->predictWithTimeAndDistance(qDataList);          // predict exact knn
        timeList.push_back(getElapsed(start));                      // timestamp end

        // Hypercube query set running
        std::cout << " Running query set in CurvesLsh-Hypercube ANN..." << newline;
        start = initTime();                                         // timestamp start
        A = clCube->predictWithTimeAndDistance(qDataList);           // predict Approx knn
        timeList.push_back(getElapsed(start));                      // timestamp end

        /// Calculate results and stats also
        results = unrollResult<Y, TX, CY> (E, A, qLabelList, "CurvesLSH - Hypercube");       // unroll the results and the stats to tuple strings.
        timeList.push_back(getElapsed(AppStart));                      // timestamp App End

        // Write result, stats to according files
        string fields  = "AlgorithmName,id,datetime,L,w,m,k,topLimitLsh,radius,dimension,Accuracy,Time,iFileName,trainSize,fitExactTime,predictExactTime,fitApproxTime,predictApproxtime,AppTime\n";
        oFile.open("stats_Cube.csv", std::ios::out | std::ios::app);
        if ( ! oFile.is_open() ) {
            break;
        }
        string res;
        res = "CurvesLsh-Cube," + to_string(id) + "," + getDatetime(false) + ", L_grid = " + to_string(L_grid) + ", w = " + to_string(w) + ", probes = " +
              to_string(probes) + ", k = " + to_string(k) + ", M = " + to_string(M) + ", dimension = " + to_string(dimension) + "," +
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
    delete clCube;
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
    if ( readCurvesLSHWithHypercubeParameters(argc, argv, iFileName, qFileName, oFileName, k, M, probes, L_grid) != 0  ){
        return -1;
    }

    // Run CurveLSH - Hypercube
    runCurveCube<double>(id,iFileName, qFileName, oFileName, metric_name, L_grid, k, M, w, probes);

    return 0;
}

void runCurveLSHWithHypercube( std::string &iFileName, std::string &qFileName, std::string &oFileName, std::string metric_name="euclidean",
                               int L_grid = 4, int k =3, int M =10, double w=3000, int probes = 2 ){

    using namespace std;

    typedef vector<double> PointX;
    typedef vector<PointX> X; // X is a Curve
    typedef list<X> CX;
    typedef list<string> CY;
    typedef string Y;
    typedef double TX;
    tuple<double, list<vector<int>>> CostAndPath;


//    typedef LSH<X, TX, Y> LSH_;
//    typedef ExactKNeighbors<CX, X, TX, CY, Y> EKNN_;
    typedef chrono::steady_clock::time_point timePoint;
    // these list are returned by predictWithTime method.
//    std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> A, E;

    timePoint start; // start variable for each method execution.
    timePoint AppStart = initTime(); // start variable for whole program.
    int dimension;
    string newline = "\n", space = " ", stats, result;
    tuple<string, string> results;

    // declare train list, test train list, query list, test query list
    CX iDataList, qDataList, testIDataList, testQDataList;
    CY iLabelList, qLabelList, testILabelList, testQLabelList;
    vector<double> timeList;                                    // This vector is used to store time values.
//    string oFileName;
    ofstream oFile;

    // Read Train data and query data.
    start = initTime();                                         // timestamp start
    readTrajectories<CX, CY, X, TX>(iFileName, iDataList, iLabelList);
    readTrajectories<CX, CY, X, TX>(qFileName, qDataList, qLabelList);

    // Get points dimension and min and max number of points in curves dataset
    int dim = iDataList.begin()->begin()->size();
    // Max number of points
    CX::iterator max_itr = max_element(iDataList.begin(),iDataList.end(),longestVec<double>);
    int max_curve_sz = max_itr->size();
    // Min number of points
    CX::iterator min_itr = max_element(iDataList.begin(),iDataList.end(),shortestVec<double>);
    int min_curve_sz = min_itr->size();
    // Set the max curve size to use
    int maxPointsInCurves = 1.1 * max_curve_sz;
    // Set delta to be used in curves LSH
    double delta = 0.001; //4 * dim * min_curve_sz;
    // Maximum number in coordinates
    double maxCoordNum=55;

    // Create LSH for curves structure
    typedef double HQ_DX;
    typedef std::string HQ_DY;
    typedef std::vector<double> HQ_IDX;
//    typedef std::tuple<X,HQ_DY> HQ_SAVEDDATA;
////    typedef Hypercube<HQ_IDX, double, HQ_SAVEDDATA> HQ_Struct;
    typedef Hypercube< std::vector<double>, double, std::tuple< std::vector< std::vector<double> >, std::string >  > HQStruct;
    typedef LSH< std::vector<double>, double, std::tuple< std::vector< std::vector<double> >, std::string >  > LSHStruct;
    typedef CurvesLSH< HQ_DX, HQ_DY, Hypercube< std::vector<double>, double, std::tuple< std::vector< std::vector<double> >, std::string >  > > CLSH_HQ_Struct;
    typedef CurvesLSH< HQ_DX, HQ_DY, LSHStruct > CLSH_LSH_Struct;

//     Uncomment following line for running CurvesLSH with Hypercube
    CLSH_HQ_Struct* CLSH = new CLSH_HQ_Struct(dim, delta, k, M, probes, maxPointsInCurves, L_grid , w , metric_name, maxCoordNum );

////     Uncomment following line for running CurvesLSH with LSH
//    CLSH_LSH_Struct* CLSH = new CLSH_LSH_Struct(dim, delta, k, maxPointsInCurves, L_grid , w , metric_name, maxCoordNum);

    CX::iterator CurveIter;
    CY::iterator CurveLblIter;
    // Add all train dataset curves to CLSH structure
    int i =0;
    for (CurveIter = iDataList.begin(), CurveLblIter = iLabelList.begin(); CurveIter != iDataList.end(), CurveLblIter != iLabelList.end() ; CurveIter++, CurveLblIter++ ) {
        i++;
        CLSH->addX(*CurveIter, *CurveLblIter);
    }

    CurveIter = qDataList.begin();

    std::list<std::tuple<std::string, double> > queryRes, queryResList;
//    std::list< std::tuple< std::string, std::vector<std::vector<double> > > > res =
    for (CurveIter = qDataList.begin(), CurveLblIter = qLabelList.begin(); CurveIter != qDataList.end(), CurveLblIter != qLabelList.end() ; CurveIter++, CurveLblIter++ ) {
        i++;
        queryRes = CLSH->queryX(*CurveIter);
        queryResList.push_back( queryRes.front());
    }

//    CLSH->queryX((*CurveIter));

    // Clean
    delete CLSH;

    std::cout << "max curves points " << max_curve_sz <<endl ;

}



int iimain(int argc, char **argv) {

    using namespace std;
    // Set program parameters
    int k = 4, M = 10, probes =2, L_grid = 4;
    double w = 0.01;
    int id = 0;
    char *pEnd;
    const char *arg;
    string newline = "\n", space = " ", metric_name = "euclidean", stats, result;
    string iFileName, qFileName, oFileName;

    // Read/Verify program parameters
    if ( readCurvesLSHWithHypercubeParameters(argc, argv, iFileName, qFileName, oFileName, k, M, probes, L_grid) != 0 ){
        return -1;
    }

    runCurveLSHWithHypercube(iFileName, qFileName, oFileName, metric_name, L_grid, k, M, (double) w, probes);


    return 0;
}

