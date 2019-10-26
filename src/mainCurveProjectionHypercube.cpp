//
// Created by ody on 17/10/19.
//

#include<cstring>
#include "../inc/KNeighborsClassifier.h"
#include "../inc/PathFinder.h"
#include"../inc/Projection.h"

void
runCurveProjectionHypercube(int id, std::string &iFileName, std::string &qFileName, std::string &outFile, int L = 5,
                            int k = 4, int w = 6000, int numNeighbors = 1, int topLimi = 4, int m = 0, int probes = 0,
                            int M = 0) {
    /**
     * @brief Runs lsh knn algorithm.
     * @params TODO complete.
     */

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
    typedef std::tuple<std::string, std::vector<std::vector<double>> &> Y_Curve_ANN;
    // these list are returned by predictWithTime method.
//    std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> A, E;

    timePoint start; // start variable for each method execution.
    timePoint AppStart = initTime(); // start variable for whole program.
    int topLimit = topLimi, dimension = 6;
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
    readTrajectories<CX, CY, X, TX>(iFileName, iDataList, iLabelList);
    readTrajectories<CX, CY, X, TX>(qFileName, qDataList, qLabelList);

    // Get points dimension and min and max number of points in curves dataset
    int dim = iDataList.begin()->begin()->size();
    // Set the max curve size to use

    // Reduce Curves to dimension 25..
    ReduceTrajectories<CX,CY,X>(iDataList,iLabelList,testIDataList, testILabelList, dimension, iDataList.size());
    // take the last 100 records of curves for query set.
    ReduceTrajectories<CX,CY,X>(iDataList,iLabelList,testQDataList, testQLabelList, dimension, iDataList.size()/10);
    // Max number of points
    CX::iterator max_itr = max_element(testIDataList.begin(),testIDataList.end(),longestVec<double>);
    int max_curve_sz = max_itr->size();
    // Min number of points
    CX::iterator min_itr = max_element(testIDataList.begin(),testIDataList.end(),longestVec<double>);
    int min_curve_sz = min_itr->size();
    int kk = testQDataList.size();
list<list<vector<int>>> paths;
    iDataList.pop_front();
//    CostAndPath = dtwWindow<X, PointX, TX>(iDataList.front(), qDataList.front(), 3, 2);
    PathFinder *pathFinder = new PathFinder(3, 7, 2, 0 );
//    for (int i = 0; i < 10; ++i) {
//        for (int j = 0; j < 10; ++j) {
//            pathFinder->setNM(i,j);
//       paths = pathFinder->RelevantPaths();
//        }
//    }
//    PathFinder *pathFinder = new PathFinder(1, 10, 2, 0 );
//    paths = pathFinder->RelevantPaths();
    int i = 0;
    for (auto & path : paths) {
        cout << i << ".\t\t";
        for (auto t : path) {
            cout << "(" << t[0] << ", " << t[1] << "), ";
        }
        i++;
        cout << endl;
    }
    cout << paths.size() << ", totalpaths "  << pathFinder->getSize() << endl;
    cout << endl;
    pathFinder->PrintTable();
//    auto first = std::make_tuple(1, iDataList);
    iDataList.pop_front();
//    qDataList.pop_front();
    CostAndPath = dtw<X, PointX, TX>(iDataList.front(), qDataList.front(), "euclidean");
    cout << "Time to read files : " << getElapsed(start) << " list Sizes " << iDataList.size() << " "
         << iLabelList.size() << " " << qDataList.size() << endl;
    cout << "Distance of first Points: " << get<0>(CostAndPath) << " and Path:\n";
    for (auto item : get<1>(CostAndPath)) {
        cout << "( " << item[0] << ", " << item[1] << "), ";
//        i++;
    }
    cout << endl;
//    exit(1);
    typename CX::iterator iterData1; // some iterators
    typename CY::iterator iterLabel1;
    typename CX::iterator iterQData;
    typename CY::iterator iterQLabel;
    typedef Projection<double, std::string, LSH<std::vector<double>, double, Y_Curve_ANN>>  PR_;
    PR_ * pr_lsh = new PR_(dimension, dim, 6000, 4, L=1, 0, 0, 100, "euclidean", 0.5);

    CX::iterator CurveIter;
    CY::iterator CurveLblIter;
    // Add all train dataset curves to CLSH structure
    for (CurveIter = iDataList.begin(), CurveLblIter = iLabelList.begin();
         CurveIter != iDataList.end(), CurveLblIter != iLabelList.end() ; CurveIter++, CurveLblIter++ ) {
        pr_lsh->addX(*CurveIter, *CurveLblIter);
    }


    std::cout << "max curves points " << max_curve_sz <<endl ;
//    Projection(int dimensionCurve, int dimensionPoint, int w = 6000, int k = 4, int L = 5, int m = 0, double radius = 0,
//            int top_limit = 0, std::string metric_name = "euclidean");
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

int main(int argc, char **argv) {
    using namespace std;

    int L1, k1, L = 5, k = 4, w = 5000, numNeighbors = 1, topLimit =
            4 * L, probes, r = 0, dimension = 0, M = 0, m = 0;// INT32_MAX - 5;
    int id = 0;
    char *pEnd;
    const char *arg;
    string newline = "\n", space = " ", metric_name = "manhattan", stats, result;
    string oFileName, iFileName, qFileName, output;

    //Read args
    if (argc != 15) {
        fprintf(stderr, "Usage : %s -d <input file> -q <query file> -k <int> -L <int> -o <output file>\n", argv[0]);
        return 1;
    }
    // -d tests/sample_datasets/trajectories_dataset -q tests/sample_datasets/trajectories_test1 -k_hypercube 4 -probes 5 -M 5 -L_grid 6 -o tests/traj_output1.txt
    while (--argc && argc > 8) {
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

    runCurveProjectionHypercube(0, iFileName, qFileName, output, L, k, w, numNeighbors, topLimit, m, M);


    return 0;
}
