//
// Created by user on 10/28/19.
//

#include<cstring>
#include "../inc/KNeighborsClassifier.h"
#include "../inc/PathFinder.h"
#include"../inc/Projection.h"

void
runCurveProjectionLsh(std::string &iFileName, std::string &qFileName, std::string &oFileName, int k_vec = 4,
                      int L_vec = 5, int w = 6000, int m = 0, int topLimit = 0, double e = 0.5) {
    /**
     * @brief Runs lsh knn algorithm.
     * @params TODO complete.
     */

    using namespace std;
    typedef double DX;
    typedef vector<DX> PointX;
    typedef vector<PointX> X; // X is a Curve
    typedef list<X> CX;
    typedef list<string> CY;
    tuple<double, list<vector<int>>> CostAndPath;
    typedef chrono::steady_clock::time_point timePoint;
    typedef std::string DY;
    typedef std::vector<double> HQ_IDX;
    typedef std::tuple<X, DY> HQ_SAVEDDATA;
    typedef ExactKNeighbors2<CX, X, DX, CY, DY> ENNStruct;
    typedef Hypercube<PointX, DX, HQ_SAVEDDATA> HQStruct;
    typedef LSH<PointX, DX, HQ_SAVEDDATA> LSHStruct;
    typedef Projection<DX, DY, HQStruct> ProjectionCurveHQStruct;
    typedef Projection<DX, DY, LSHStruct> ProjectionCurveLSHStruct;
    // these list are returned by predictWithTime method.
    std::list<std::tuple<double, std::list<std::tuple<DY, DX>>>> ANNResults, ENNResults;

    timePoint start; // start variable for each method execution.
    timePoint AppStart = initTime(); // start variable for whole program.
    int dimension = 6;
    string newline = "\n", space = " ", metric_name = "euclidean", stats, result;
    tuple<string, string> results;
    // declare train list, test train list, query list, test query list
    CX iDataList, qDataList, testIDataList, testQDataList;
    CY iLabelList, qLabelList, testILabelList, testQLabelList;
    vector<double> timeList;                                    // This vector is used to store time values.
//    string oFileName, output;
    ofstream oFile;

    // Ask for input file if not given in arguments
    if (inputFileMessageDialog("Please provide the training dataset file path (or file name if in running directory)",
                               "No input file has been provided! Program terminates..please try again",
                               iFileName) != 0) { exit(EXIT_FAILURE); }
    start = initTime();                                         // timestamp start
    // Read Train data
    readTrajectories<CX, CY, X, DX>(iFileName, iDataList, iLabelList);
//    readTrajectories<CX, CY, X, DX>(qFileName, qDataList, qLabelList);

    // Get points dimension and min and max number of points in curves dataset
    int dim = iDataList.begin()->begin()->size();
    // Set the max curve size to use

    // Reduce Curves to dimension 25..
    ReduceTrajectories<CX, CY, X>(iDataList, iLabelList, testIDataList, testILabelList, dimension,
                                  iDataList.size() / 10);
    // Max number of points
    CX::iterator max_itr = max_element(testIDataList.begin(), testIDataList.end(), longestVec<double>);
    int max_curve_sz = max_itr->size();
    // Min number of points
    CX::iterator min_itr = max_element(testIDataList.begin(), testIDataList.end(), longestVec<double>);
    int min_curve_sz = min_itr->size();
    CX::iterator CurveIter;
    CY::iterator CurveLblIter;
    ProjectionCurveLSHStruct *projLSH = new ProjectionCurveLSHStruct(dimension, dim, w, k_vec, L_vec, 0, 0, topLimit, e,
                                                                     "euclidean");
    auto *clProjLSH = new KNeighborsClassifier<ProjectionCurveLSHStruct *, CX, X, DX, CY, DY>(projLSH);

    dimension = iDataList.front().size(); // get dimension of data
    // Initialize algorithms
    auto *eknn = new ENNStruct(0, metric_name);
    auto *clEknn = new KNeighborsClassifier<ENNStruct *, CX, X, DX, CY, DY>(eknn);
    /// Start fit and predict

    std::cout << " Fitting train set to Exact KNN..." << newline;
    start = initTime();                                         // timestamp start
    clEknn->fit(testIDataList, testILabelList);                         // fit exact knn
    timeList.push_back(getElapsed(start));                      // timestamp end

    std::cout << " Fitting train set to LSH ANN..." << newline;
    start = initTime();                                         // timestamp start
    clProjLSH->fit(testIDataList, testILabelList);                          // fit approx knn
    timeList.push_back(getElapsed(start));                      // timestamp end

    // User's answer on repeated procedure (querying with new file)
    std::string userAnswer = "Y";

    // Repeat until user terminates
    while (userAnswer == "Y" || userAnswer == "y") {

        // Ask for query file
        if (inputFileMessageDialog("Please provide the query dataset file path (or file name if in running directory)",
                                   "No query file has been provided! Program terminates..please try again",
                                   qFileName) != 0) { break; }

        // Read Query data
        std::cout << "Reading querying data..." << '\n';
        readTrajectories<CX, CY, X, DX>(qFileName, qDataList, qLabelList);
        // take the last 100 records of curves for query set.
        ReduceTrajectories<CX, CY, X>(qDataList, qLabelList, testQDataList, testQLabelList, dimension,
                                      100);

        // Ask for output file
        if (inputFileMessageDialog("Please provide the output file path (or file name if in running directory)",
                                   "No output file has been provided! Program terminates..please try again",
                                   oFileName) != 0) { break; }

        // Inform user for running case
        std::cout << "Running CurvesLSH-Hypercube Projection with arguments: " << "input file = \"" << iFileName
                  << "\" query file = \"" << qFileName << "\" output file =  \"" << oFileName
                  << " k = " << k_vec << " L = " << L_vec << " e = " << e << "\n";

        // KNN query set running
        std::cout << " Running query set in Exact KNN..." << newline;
        start = initTime();                                         // timestamp start
        ENNResults = clEknn->predictWithTimeAndDistance(testQDataList);          // predict exact knn
        timeList.push_back(getElapsed(start));                      // timestamp end
        // Hypercube query set running
        std::cout << " Running query set in CurvesLsh-Hypercube Projection ANN..." << newline;
        start = initTime();                                         // timestamp start
        ANNResults = clProjLSH->predictWithTimeAndDistance(testQDataList);           // predict Approx knn
        timeList.push_back(getElapsed(start));                      // timestamp end
        /// Calculate results and stats also
        results = unrollResult<DY, DX, CY>(ENNResults, ANNResults,
                                           testQLabelList);       // unroll the results and the stats to tuple strings.
        timeList.push_back(getElapsed(AppStart));                      // timestamp App End

        // Write result, stats to according files
        string fields = "AlgorithmName,id,datetime,L,w,m,k,topLimit,numNeighbors,dimension,Accuracy,Time,iFileName,trainSize,fitExactTime,predictExactTime,fitApproxTime,predictApproxtime,AppTime\n";
//    oFile.open("tests/stats_proj.csv", std::ios::out | std::ios::app);
        string res = "HC, 0," + getDatetime(false) + "," + to_string(L_vec) + "," + to_string(w) + "," +
                     to_string(m) + "," + to_string(k_vec) +
                     "," + to_string(topLimit) + "," + to_string(0) + "," + to_string(dimension) + "," +
                     get<1>(results) + "," + getFilename(iFileName) +
                     "," + to_string(testIDataList.size()) + "," + to_string(timeList[0]) + "," +
                     to_string(timeList[2]) +
                     "," + to_string(timeList[1]) + "," + to_string(timeList[3]) + "," + to_string(timeList[4]) +
                     "\n";
//    oFile << res;
//    oFile.close();
        cout << getDatetime(false) << "\t\t\t\t" << fields;
        cout << getDatetime(false) << "\t\t\t\t" << res;

//    oFile.open("tests/lsh_results/test3/out/Output_proj_HC__" + getDatetime(true) + ".txt", std::ios::out);
//    oFile << get<0>(results);
//    oFile << "\n" + get<1>(results) + "\n";
//    oFile.close();
        oFile.open(oFileName, std::ios::out);
        oFile << get<0>(results);
        oFile << "\n" + get<1>(results) + "\n";
        oFile.close();

        // Ask user for repeating
        std::cout << "Would you like to continue with a new query file? (Y/N)" << '\n';
        std::cin >> userAnswer;
        if (userAnswer == "N" || userAnswer == "n") { break; }

        // Clear files to repeat
        qFileName = "";
        oFileName = "";

        // Clear query structures to start again
        qDataList.clear();
        qLabelList.clear();

    }
//    // Clean Up
//
    delete eknn;
    delete clEknn;
    delete projLSH;
    delete clProjLSH;
}

int main(int argc, char **argv) {
    using namespace std;

    int L = 1, k_vec = 4, w = 5000, topLimit = 4 * 100, m = 0;
    char *pEnd;
    const char *arg;
    double e = 0.5;
    string newline = "\n", space = " ", metric_name = "manhattan", stats, result;
    string oFileName, iFileName, qFileName, output;

    //Read args
    if (argc != 13) {
        fprintf(stderr, "Usage : %s -d <input file> -q <query file> -k_hypercube <int> -L <int> -o <output file>\n",
                argv[0]);
        return 1;
    }
    // -d tests/sample_datasets/trajectories_dataset -q tests/sample_datasets/trajectories_test1 -k_hypercube 4 -probes 5 -M 5 -L_grid 6 -o tests/traj_output1.txt
    while (--argc && argc > 4) {
        arg = *++argv;
        if (arg == NULL) break;
        if (!strcmp(arg, "-o")) {
            if (argc > 1 && --argc)
                oFileName = *++argv;
        } else if (!strcmp(arg, "-e"))
            e = (double) stod(*++argv);
        else if (!strcmp(arg, "-L_vec"))
            L = (int) strtol(*++argv, &pEnd, 10);
        else if (!strcmp(arg, "-k_hypercube"))
            k_vec = (int) strtol(*++argv, &pEnd, 10);
        else if (!strcmp(arg, "-q"))
            qFileName = *++argv;
        else if (!strcmp(arg, "-d"))
            iFileName = *++argv;
    }

    runCurveProjectionLsh(iFileName, qFileName, oFileName, k_vec, L, w, m, topLimit, e);
//    runCurveProjectionLsh(std::string &iFileName, std::string &qFileName, std::string &outFile, int k_vec = 4, int L_vec = 5,int w = 6000, int m = 0, int topLimit = 0, double e = 0.5) {


    return 0;
}
