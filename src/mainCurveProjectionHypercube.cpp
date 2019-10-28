//
// Created by ody on 17/10/19.
//

#include<cstring>
#include "../inc/KNeighborsClassifier.h"
#include "../inc/PathFinder.h"
#include"../inc/Projection.h"

void
runCurveProjectionLsh(std::string &iFileName, std::string &qFileName, std::string &oFileName, int k = 4, int k_hi = 4,
                      int w = 6000, int m = 0, int probes = 0, int M = 0, double e = 0.5) {
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

    // Read Train data
    start = initTime();                                         // timestamp start
    readTrajectories<CX, CY, X, DX>(iFileName, iDataList, iLabelList);
//    readTrajectories<CX, CY, X, DX>(qFileName, qDataList, qLabelList);

    // Get points dimension and min and max number of points in curves dataset
    int dim = iDataList.begin()->begin()->size();
    // Set the max curve size to use

    // Reduce Curves to dimension 25..
    ReduceTrajectories<CX, CY, X>(iDataList, iLabelList, testIDataList, testILabelList, dimension,
                                  iDataList.size() / 1000);
    // take the last 100 records of curves for query set.
//    ReduceTrajectories<CX,CY,X>(iDataList,iLabelList,testQDataList, testQLabelList, dimension, iDataList.size()/7000);
    // Max number of points
    CX::iterator max_itr = max_element(testIDataList.begin(), testIDataList.end(), longestVec<double>);
    int max_curve_sz = max_itr->size();
    // Min number of points
    CX::iterator min_itr = max_element(testIDataList.begin(), testIDataList.end(), longestVec<double>);
    int min_curve_sz = min_itr->size();
    CX::iterator CurveIter;
    CY::iterator CurveLblIter;
    ProjectionCurveHQStruct *projHC = new ProjectionCurveHQStruct(dimension, dim, w, k, M, probes, DBL_MAX, e,
                                                                  "euclidean");
    auto *clProjHC = new KNeighborsClassifier<ProjectionCurveHQStruct *, CX, X, DX, CY, DY>(projHC);

    dimension = iDataList.front().size(); // get dimension of data
    // Initialize algorithms
    auto *eknn = new ENNStruct(0, metric_name);
    auto *clEknn = new KNeighborsClassifier<ENNStruct *, CX, X, DX, CY, DY>(eknn);
    /// Start fit and predict

    std::cout << " Fitting train set to Exact KNN..." << newline;
    start = initTime();                                         // timestamp start
    clEknn->fit(testIDataList, testILabelList);                         // fit exact knn
    timeList.push_back(getElapsed(start));                      // timestamp end

    std::cout << " Fitting train set to HC Projection ANN..." << newline;
    start = initTime();                                         // timestamp start
    clProjHC->fit(testIDataList, testILabelList);                          // fit approx knn
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
        std::cout << "Running CurvesHypercube Projection with arguments: " << "input file = \"" << iFileName
                  << "\" query file = \"" << qFileName << "\" output file =  \"" << oFileName
                  << " k_hy = " << k << " M = " << M << "probes = " << probes << " e = " << e << "\n";

        // KNN query set running
        std::cout << " Running query set in Exact KNN..." << newline;
        start = initTime();                                         // timestamp start
        ENNResults = clEknn->predictWithTimeAndDistance(testQDataList);          // predict exact knn
        timeList.push_back(getElapsed(start));                      // timestamp end

        std::cout << " Running query set in CurveHypercube Projection ANN..." << newline;
        start = initTime();                                         // timestamp start
        ANNResults = clProjHC->predictWithTimeAndDistance(testQDataList);           // predict Approx knn
        timeList.push_back(getElapsed(start));                      // timestamp end
        /// Calculate results and stats also
        results = unrollResult<DY, DX, CY>(ENNResults, ANNResults,
                                           testQLabelList);       // unroll the results and the stats to tuple strings.
        timeList.push_back(getElapsed(AppStart));                      // timestamp App End

        // Write result, stats to according files
        string fields = "AlgorithmName,id,datetime,probes,w,m,k,topLimit,numNeighbors,dimension,Accuracy,Time,iFileName,trainSize,fitExactTime,predictExactTime,fitApproxTime,predictApproxtime,AppTime\n";
//    oFile.open("tests/stats_proj.csv", std::ios::out | std::ios::app);
        string res = "HC, 0," + getDatetime(false) + "," + to_string(probes) + "," + to_string(w) + "," +
                     to_string(m) + "," + to_string(k) +
                     "," + to_string(M) + "," + to_string(0) + "," + to_string(dimension) + "," +
                     get<1>(results) + "," + getFilename(iFileName) +
                     "," + to_string(iDataList.size()) + "," + to_string(timeList[0]) + "," + to_string(timeList[1]) +
                     "," + to_string(timeList[2]) + "," + to_string(timeList[3]) + "," + to_string(timeList[4]) +
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
    delete eknn;
    delete clEknn;
    delete projHC;
    delete clProjHC;
}

int main(int argc, char **argv) {
    using namespace std;

    int k_hypercube = 4, w = 5000,probes, M = 0, m = 0;
    char *pEnd;
    const char *arg;
    double e;
    string newline = "\n", space = " ", metric_name = "manhattan", stats, result;
    string oFileName, iFileName, qFileName, output;

    //Read args
    if (argc != 15) {
        fprintf(stderr, "Usage : %s -d <input file> -q <query file> -k_hypercube <int> -L <int> -o <output file>\n",
                argv[0]);
        return 1;
    }
    // -d tests/sample_datasets/trajectories_dataset -q tests/sample_datasets/trajectories_test1 -k_hypercube 4 -probes 5 -M 5 -L_grid 6 -o tests/traj_output1.txt
    while (--argc && argc > 3) {
        arg = *++argv;
        if (arg == NULL) break;
        if (!strcmp(arg, "-o")) {
            if (argc > 1 && --argc)
                oFileName = *++argv;
        } else if (!strcmp(arg, "-e"))
            e = (double) stod(*++argv);
        else if (!strcmp(arg, "-M"))
            M = (int) strtol(*++argv, &pEnd, 10);
        else if (!strcmp(arg, "-k_hypercube"))
            k_hypercube = (int) strtol(*++argv, &pEnd, 10);
        else if (!strcmp(arg, "-probes"))
            probes = (int) strtol(*++argv, &pEnd, 10);
        else if (!strcmp(arg, "-q"))
            qFileName = *++argv;
        else if (!strcmp(arg, "-d"))
            iFileName = *++argv;
    }

//    runCurveProjectionLsh(std::string &iFileName, std::string &qFileName, std::string &oFileName, int k_hypercube = 4, int k_hi = 4, int w = 6000, int m = 0, int probes = 0, int M = 0) {
    runCurveProjectionLsh(iFileName, qFileName, oFileName, k_hypercube, w, m, M, probes, e);


    return 0;
}
