//
// Created by ody on 13/10/19.
//

#ifndef ADS_PROJECT1_KNEIGHBORSCLASSIFIER_H
#define ADS_PROJECT1_KNEIGHBORSCLASSIFIER_H

#include "LSH_HT.h"


/********************** ExactKNeighbors Part**********************************************************/
template<class TD, class TID, class D, class TY, class Y>
/*Usually TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
class ExactKNeighbors {
    /* This class is used as an algorithm for exact NN and has 2 methods addPoint, QueryPoint */
    int n_neighbors;            // Number of Neighbors to search.
    std::string metric_name;    // metric that will be used.
    TD data;                    // object that will store the data
    TY labels;                  // object that will store the labels

    D (*f)(TID &, TID &); /* This is the function Pointer to selected metric its declaration is here
                     * and the definition an initialization*/
public:
    ExactKNeighbors(int n_neighbors, std::string metric) : n_neighbors(n_neighbors), metric_name(metric) {
        /* Constructor and initialization */
        if (metric_name == "manhattan")     /* definition of the metric depending */
            f = &manhattanDistance<D, TID>;  /* on the metric_name argument passed to the constructor*/
    }

    void addPoint(TID &, Y &); // Add a Vector of int with its label
    std::list<std::tuple<Y, D>> queryPoint(TID &x); // Query a Point it return a list of tuples (label, distance)
};

template<class TD, class TID, class D, class TY, class Y>
/*Usually TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
void ExactKNeighbors<TD, TID, D, TY, Y>::addPoint(TID &x, Y &y) {
    /**
     * @brief Add key and value to according list.
     * @param x The first object with data.
     * @param y The second object with label.
     * @return void.
     */
    data.push_back(x);
    labels.push_back(y);
}

template<class TD, class TID, class D, class TY, class Y>
/*Usually TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
std::list<std::tuple<Y, D>> ExactKNeighbors<TD, TID, D, TY, Y>::queryPoint(TID &x) {
    /**
     * @brief Query a point.
     * @param x The object with data that the query will be executed.
     * @return A newly-constructed list, containing tuples<label,distance>.
     */

    typedef typename TD::iterator tdIt; // Iterator on the list of vectors
    typedef typename TY::iterator tyIt; // iterator on the list of strings
    tdIt iteratorData; // Init Iterator on list of vectors
    tdIt itDE = data.end(); // end of data iterator
    tyIt iteratorLabels; // Iterator on the list of strings



    typedef std::list<std::tuple<Y, D>> listTuples;
    typedef typename listTuples::iterator IteratorListTuples;

    listTuples distanceList;
    listTuples labelDistanceList;
    IteratorListTuples iterListTuples;
    int j;
    // Calculate distance from query point against all data.
    for (iteratorData = data.begin(), iteratorLabels = labels.begin();
         iteratorData != itDE; ++iteratorData, ++iteratorLabels) {
        distanceList.push_back(std::make_pair(*iteratorLabels, f(*iteratorData, (x)))); // Append distances to a list.
    }
    distanceList.sort(TupleLess<1>()); // sort distance list by neighbors
    IteratorListTuples itE = distanceList.end();
    // Now append the nearest neighbors to the return list
    for (j = 0, iterListTuples = distanceList.begin(); (j < this->n_neighbors) && (iterListTuples != itE); ++j, ++iterListTuples) {
        labelDistanceList.push_back(*iterListTuples);
    }

    return labelDistanceList;
}

/********************** KNeighbors Classifier Part**********************************************************/
/* A:lsh, cube,bruteForce TD: type of data list, vector  etc TY type of labels */
template<class A, class TD, class TID, class D, class TY, class Y>
/*Usually A:Algorithm to run class, TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
class KNeighborsClassifier {
    int n_neighbors;
    std::string algorithm_name;
    std::string metric_name;
    A alg;
    TD data;
    TY labels;

public:

    KNeighborsClassifier(int n_neighbors, A &alg) : n_neighbors(n_neighbors), alg(alg) {
        /**
         * @brief Constructor of this object.
         * @param n_neighbors The integer with the number of max neighbors to search.
         * @param alg The algorithm that will be used to classify data.
         * @return void.
         */
    }

    void fit(TD &x, TY &y); // Fit data of list<vector<int>>, and list< string>
    /* This method is used to get prediction not only for label but also for time and distance, specific for this
     * homework */
    std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> predictWithTimeAndDistance(TD &x);
    /* This method is a test method for simple predict, just return list of labels*/
//    predict(TD &x); // return a list of lists that each list contains tuple(x.name, y.name), tuple(x.name, y.name)
};


template<class A, class TD, class TID, class D, class TY, class Y>
/*Usually A:Algorithm to run class, TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
void KNeighborsClassifier<A, TD, TID, D, TY, Y>::fit(TD &x, TY &y) {
    /**
     * @brief General fit for all methods lsh, cube, exactKnn.
     * @param x The first object with data.
     * @param y The second object with label.
     * @return void.
     */

    typedef typename TD::iterator tdIt; // Iterator on the list of vectors
    typedef typename TY::iterator tyIt; // iterator on the list of strings
    tdIt iteratorData; // Init Iterator on list of vectors
    tdIt itDE = data.end(); // end of data iterator
    tyIt iteratorLabels; // Iterator on the list of strings
    data = x;
    labels = y;
    // fit all data to alg algorithm's structure.
    for (iteratorData = data.begin(), iteratorLabels = labels.begin();
         iteratorData != itDE; ++iteratorData, ++iteratorLabels) {
        alg->addPoint(*iteratorData, *iteratorLabels); // Add a vector<int> and string
    }

}

template<class A, class TD, class TID, class D, class TY, class Y>
/*Usually A:Algorithm to run class, TD: list<vector<int>>, TID: vector<int>, D: int, TY list<string>, Y string*/
std::list<std::tuple<double, std::list<std::tuple<Y, D>>>>
KNeighborsClassifier<A, TD, TID, D, TY, Y>::predictWithTimeAndDistance(TD &x) {
    /**
     * @brief General fit for all methods lsh, cube, exactKnn.
     * @param x The first object with data.
     * @return A newly-constructed list of tuples (timeValue, list(tuple<label, distance>))
     *
     * Each record of tuple is (timeValue, list(tuple(label,distanceValue))
     * because each query has a time value and K neighbors, so will have a list of these tuples
     * and for all queries a list of of tuples(time, listOfNeighbors)
     *
     * TODO delete Count CPU+WALL Time https://stackoverflow.com/questions/2808398/easily-measure-elapsed-time
     */
    typedef typename TD::iterator IteratorTD; // Iterator typedef on data
    typedef std::list<std::tuple<Y, D>> listTuples; // list of tuples <label,distances> , needed to calculcate neighbors
    typedef typename listTuples::iterator lTIt; // Iterator typedef on list of tuples
    typedef std::chrono::steady_clock::time_point timePoint;
    typedef std::list<std::tuple<double, std::list<std::tuple<Y, D>>>> returnL; // typedef the return type because its big

    timePoint start;
    double elapsed;
    listTuples distanceList; // distanceList to store all neighbors
    lTIt iteratorListTuples; // Iterator on list of tuples
    returnL returnList; // definition of return list

    IteratorTD e1 = x.end();
    IteratorTD iteratorData;
    for (iteratorData = x.begin(); iteratorData != e1; ++iteratorData) {
        listTuples labelDistanceList; // every query has new labelDistanceList
        start = initTime(); // Start Time
        labelDistanceList = alg->queryPoint(*iteratorData); // Query the point here send a vector<int>
        elapsed = getElapsed(start); // End Time
        returnList.push_back(std::make_pair(elapsed, labelDistanceList)); // Append to the return list the time and the distance list return by alg query method
    }

    return returnList;
}

template<typename D>
void
runLSH(int id, std::string &iFileName, std::string &qFileName, std::string &outFile, int L = 5, int k = 4, int w = 5000,
       int numNeighbors = 1, int topLimi = 4, int m = 0) {
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
    string newline = "\n", space = " ", metric_name = "manhattan", stats, result;
    tuple<string, string> results;
    // declare train list, test train list, query list, test query list
    CX iDataList, qDataList, testIDataList, testQDataList;
    CY iLabelList, qLabelList, testILabelList, testQLabelList;
    vector<double> timeList;                                    // This vector is used to store time values.
    string oFileName, output;
    ofstream oFile;
    // Read Train data and query data.
    readDataAndLabelsFromFile2<CX, CY, X, Y>(iFileName, iDataList, iLabelList);
    readDataAndLabelsFromFile2<CX, CY, X, Y>(qFileName, qDataList, qLabelList);
    typename CX::iterator iterData1; // some iterators
    typename CY::iterator iterLabel1;
    typename CX::iterator iterQData;
    typename CY::iterator iterQLabel;

    dimension = iDataList.front().size(); // get dimension of data
    // Initialize algorithms
    LSH_ *lsh = new LSH<X, TX, Y>(w, dimension, k, L, m, numNeighbors, topLimit, metric_name);
    auto *clLsh = new KNeighborsClassifier<LSH_ *, CX, X, TX, CY, Y>(numNeighbors, lsh);
    auto *eknn = new EKNN_(numNeighbors, metric_name);
    auto *clEknn = new KNeighborsClassifier<EKNN_ *, CX, X, TX, CY, Y>(numNeighbors, eknn);
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
    start = initTime();                                         // timestamp start
    clEknn->fit(iDataList, iLabelList);                         // fit exact knn
    timeList.push_back(getElapsed(start));                      // timestamp end
    start = initTime();                                         // timestamp start
    E = clEknn->predictWithTimeAndDistance(qDataList);          // predict exact knn
//    exit(1);
    timeList.push_back(getElapsed(start));                      // timestamp end
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
    string fields  = "AlgorithmName,id,datetime,L,w,m,k,topLimitLsh,numNeighbors,dimension,Accuracy,Time,iFileName,trainSize,fitExactTime,predictExactTime,fitApproxTime,predictApproxtime,AppTime\n";
    oFile.open("tests/lsh_results/test3/stats.csv", std::ios::out | std::ios::app);
    string res;
    res = "LSH," + to_string(id) + "," + getDatetime(false) + "," + to_string(L) + "," + to_string(w) + "," +
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

    oFile.open("tests/lsh_results/test3/out/Output__" + getDatetime(true) + ".txt", std::ios::out);
    oFile << get<0>(results);
    oFile << "\n" + get<1>(results) + "\n";
    oFile.close();
//    // Clean Up
//
    delete eknn;
    delete clEknn;
    delete lsh;
    delete clLsh;
}

#endif //ADS_PROJECT1_KNEIGHBORSCLASSIFIER_H
