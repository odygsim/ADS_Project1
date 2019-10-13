//
// Created by ody on 13/10/19.
//

#ifndef ADS_PROJECT1_KNEIGHBORSCLASSIFIER_H
#define ADS_PROJECT1_KNEIGHBORSCLASSIFIER_H


#include <string>
#include <list>
#include <vector>
#include <iostream>

/* A:lsh, cube,bruteForce TD: type of data list, vector  etc TY type of labels */
template<class A, class TD, class D, class TY, class Y>
class KNeighborsClassifier {
    int n_neighbors;
    std::string algorithm_name;
    std::string metric_name;
    A &alg;
    TD &data;
    TY &labels;

public:
    KNeighborsClassifier(int n_neighbors, std::string &algorithm, std::string &metric) : n_neighbors(n_neighbors),
                                                                                         algorithm_name(algorithm),
                                                                                         metric_name(metric) {
        std::cout << "KNN Initialization with " + algorithm_name << std::endl;
    }

    void fit(TD &x, TY &y);

    std::list<TD> predict(TD x);
};


template<class A, class TD, class D, class TY, class Y>
void KNeighborsClassifier<A, TD, D, TY, Y>::fit(TD &x, TY &y) {

    data = x;
    labels = y;
    if (algorithm_name == "bruteforce")
        return;
    else {
        typedef typename std::vector<TD>::iterator tdIt;
        typedef typename std::vector<TY>::iterator tyIt;
        tdIt e1 = data->end();
        tyIt e2 = labels->end();
        tdIt dIt;
        tyIt yIt;
        for (dIt = data->begin(), yIt = labels->begin(); (dIt != e1) && (yIt != e2); ++dIt, ++yIt) {
            alg->addPoint(*dIt, *yIt);
        }
    }

}

template<class A, class TD, class D, class TY, class Y>
std::list<TD> KNeighborsClassifier<A, TD, D, TY, Y>::predict(TD x) {
//        void fit( TD & x, TY & y){

    typedef typename std::vector<TD>::iterator tdIt;
    typedef typename std::vector<TY>::iterator tyIt;
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

#endif //ADS_PROJECT1_KNEIGHBORSCLASSIFIER_H
