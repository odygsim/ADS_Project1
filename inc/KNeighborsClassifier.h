//
// Created by ody on 13/10/19.
//

#ifndef ADS_PROJECT1_KNEIGHBORSCLASSIFIER_H
#define ADS_PROJECT1_KNEIGHBORSCLASSIFIER_H


#include <string>
#include <list>
#include <utility>
#include <vector>
#include <iostream>
#include "util.h"

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
    KNeighborsClassifier(int n_neighbors, std::string algorithm, std::string metric) : n_neighbors(n_neighbors),
                                                                                       algorithm_name(
                                                                                               std::move(algorithm)),
                                                                                       metric_name(metric) {
        std::cout << "KNN Initialization with " + algorithm_name << std::endl;
    }

    void fit(TD &x, TY &y);

    std::list<std::list<std::tuple<std::string, D>>> predict(TD x);
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
std::list<std::list<std::tuple<std::string, D>>> KNeighborsClassifier<A, TD, D, TY, Y>::predict(TD x) {
    //TODO what i want to return? listof klist(label, distance)

    typedef typename TD::iterator tdIt;
    typedef typename TY::iterator tyIt;
    std::list<std::tuple<int, std::vector<D>>> distanceList;

    tdIt e1 = x.end();
    tdIt dIt;
    if (algorithm_name == "bruteforce") {
        for (dIt = x.begin(); dIt != e1; ++dIt) {
            for (unsigned long i = 0; i < data.size(); ++i)
                distanceList.push_back(std::make_pair(labels[i]->getName(),
                                                      manhattanDistance<int, std::vector<int>>(data[i]->getList(),
                                                                                               (*dIt)->getList())));
        }
            distanceList.sort(ComparatorqPoint()); //TODO fix this comparator for tuples

    }

//            qpIt itS = distanceList.begin();
//            qpIt it;
//            qpIt itE = distanceList.end();
//            for (j = 0, it = itS; (j < radius) && (it!=itE); ++j, ++it) {
//                returnList.push_back(*it);
//            }
//    it = distanceList.begin();
//    for (it = itS; (it!=itE); ++it) {
//        std::cout << "Name: " << (*it)->getName()  << ", " << (*it)->getDistance() << std:: endl;//.push_back(*it);
//    }
//    qPoint *ret = distanceList.front();
//    for (int j = 0; j < distanceList.size(); ++j) {
//        delete distanceList[i];
//    }
//            return returnList;
//            sum += abs(*it1 - *it2);
//            }
//        for (int i = 0; i < ; ++i) {
//

}
//};

#endif //ADS_PROJECT1_KNEIGHBORSCLASSIFIER_H
