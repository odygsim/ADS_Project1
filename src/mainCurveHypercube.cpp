//
// Created by aikio on 25/10/19.
//

#include <string>
#include <tuple>
#include "../inc/util2.h"
#include "../inc/CurvesLSH.h"
#include "../inc/KNeighborsClassifier.h"



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
    int delta = 4 * dim * min_curve_sz;

    // Create LSH for curves structure
    typedef double DX;
    typedef std::string DY;
    typedef std::vector<double> HQ_IDX;
    typedef std::tuple<X,DY> HQ_SAVEDDATA;
//    typedef Hypercube<HQ_IDX, double, HQ_SAVEDDATA> HQ_Struct;
    typedef Hypercube< std::vector<double>, double, std::tuple< std::vector< std::vector<double> >, std::string >  > HQStruct;
    typedef LSH< std::vector<double>, double, std::tuple< std::vector< std::vector<double> >, std::string >  > LSHStruct;
    typedef CurvesLSH< DX, DY, Hypercube< std::vector<double>, double, std::tuple< std::vector< std::vector<double> >, std::string >  > > CLSH_HQ_Struct;
    typedef CurvesLSH< DX, DY, LSHStruct > CLSH_LSH_Struct;

////     Uncomment following line for running CurvesLSH with Hypercube
//    CLSH_HQ_Struct* CLSH = new CLSH_HQ_Struct(dim, delta, k, M, probes, maxPointsInCurves, L_grid , w , metric_name);

//     Uncomment following line for running CurvesLSH with LSH
    CLSH_LSH_Struct* CLSH = new CLSH_LSH_Struct(dim, delta, k, maxPointsInCurves, L_grid , w , metric_name);

    CX::iterator CurveIter;
    CY::iterator CurveLblIter;
    // Add all train dataset curves to CLSH structure
    int i =0;
    for (CurveIter = iDataList.begin(), CurveLblIter = iLabelList.begin(); CurveIter != iDataList.end(), CurveLblIter != iLabelList.end() ; CurveIter++, CurveLblIter++ ) {
        i++;
        CLSH->addX(*CurveIter, *CurveLblIter);
//        if (i>5) break;
    }

    CurveIter = qDataList.begin();

//    std::list< std::tuple< std::string, std::vector<std::vector<double> > > > res =
    CLSH->queryX((*CurveIter));

    // Clean
    delete CLSH;

    std::cout << "max curves points " << max_curve_sz <<endl ;

}



int main(int argc, char **argv) {

    using namespace std;
    // Set program parameters
    int k = 4, w = 3000, M = 10, probes =2, L_grid = 4;
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

