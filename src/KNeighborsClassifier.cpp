//
// Created by ody on 13/10/19.
//

#include "../inc/KNeighborsClassifier.h"


#include <string>
#include <list>
#include <vector>
#include <iostream>

/* A:lsh, cube,bruteForce TD: type of data list, vector  etc TY type of labels */
template < class A, class TD, class TY>
class KNeighborsClassifier {
    int n_neighbors;
    std::string algorithm_name;
    std::string metric_name;
    A * alg;
    TD * data;
    TY * labels;

public:
    KNeighborsClassifier(int n_neighbors, std::string & algorithm, std::string & metric):n_neighbors(n_neighbors),
    algorithm_name(algorithm), metric_name(metric)
    {
       std::cout << "KNN Initialization with " + algorithm_name << std::endl;
    }

    void fit( TD & x, TY & y);
//    {
//        data = x;
//        labels = y;
//        if (algorithm_name == "bruteforce")
//            return;
//        else{
//            typedef typename std::vector<TD>::iterator  tdIt;
//            typedef typename std::vector<TY>::iterator  tyIt;
//            tdIt e1 = data.end();
//            tyIt e2 = labels.end();
//            tdIt dIt;
//            tyIt yIt;
//        for (dIt = data.begin(), yIt = labels.begin(); (dIt != e1) && (yIt != e2); ++dIt, ++yIt) {
//            alg.addPoint(*dIt,*yIt);
//        }
//        }
//
//    }
//    std::list<TD> predict(TD x)
//    {
////        void fit( TD & x, TY & y){
//
//            typedef typename std::vector<TD>::iterator  tdIt;
//            typedef typename std::vector<TY>::iterator  tyIt;
//            tdIt e1 = x.end();
//            tdIt dIt;
//
//
////            for (dIt = x.begin(), yIt = y.begin(); (dIt != e1) && (yIt != e2); ++dIt, ++yIt) {
////            sum += abs(*it1 - *it2);
////            }
////        for (int i = 0; i < ; ++i) {
////
////        }
//
////        }
//
//    }
};

//KNeighborsClassifier<class A, class T, class TD, class TY>::KNeighborsClassifier(int n_neighbors,
//                                                                                 std::string &algorithm,
//                                                                                 std::string &metric):n_neighbors {
//    int n_neighbors;
//    std::string algorithm_name;
//    std::string metric_name;
//    A alg;
//    TD data;
//    TY labels;

//template < class A, class T, class TD, class TY>
//template <>
//KNeighborsClassifier<class A, class T, class TD, class TY>::KNeighborsClassifier(int n_neighbors, std::string & algorithm, std::string & metric):n_neighbors(n_neighbors),
//                                                                                         algorithm_name(algorithm), metric_name(metric){
//        std::cout << "KNN Initialization with " + algorithm_name << std::endl;
//    }

template <>
void KNeighborsClassifier<class A, class TD, class TY>::fit( TD & x, TY & y){
        data = x;
        labels = y;
        if (algorithm_name == "bruteforce")
            return;
        else{
            typedef typename std::vector<TD>::iterator  tdIt;
            typedef typename std::vector<TY>::iterator  tyIt;
            tdIt e1 = data.end();
            tyIt e2 = labels.end();
            tdIt dIt;
            tyIt yIt;
            for (dIt = data.begin(), yIt = labels.begin(); (dIt != e1) && (yIt != e2); ++dIt, ++yIt) {
                alg.addPoint(*dIt,*yIt);
            }
        }

    }
template <>
std::list<TD> KNeighborsClassifier<class A, class TD, class TY>::) predict(TD x){
//        void fit( TD & x, TY & y){

        typedef typename std::vector<TD>::iterator  tdIt;
        typedef typename std::vector<TY>::iterator  tyIt;
        tdIt e1 = x.end();
        tdIt dIt;


//            for (dIt = x.begin(), yIt = y.begin(); (dIt != e1) && (yIt != e2); ++dIt, ++yIt) {
//            sum += abs(*it1 - *it2);
//            }
//        for (int i = 0; i < ; ++i) {
//
//        }

//        }

    }
//};
