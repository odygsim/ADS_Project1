//
// Created by ody on 20/10/19.
//

#ifndef ADS_PROJECT1_PROJECTION_H
#define ADS_PROJECT1_PROJECTION_H

#include <random>
#include "util.h"

class Projection {
    std::vector<std::vector<double>> G; // keep G matrix here
    unsigned int d; // dimension , column limit of G
    unsigned int K; // row limit of G
public:
    Projection(int d);


};


Projection::Projection(int d) {
    const int rangeFrom = 0, rangeTo = 1;
    // TODO calculate K = -dloge/e^2
    // Create G Array
    std::random_device randDev;
    std::mt19937 generator(randDev());
    std::normal_distribution<double> distribution(rangeFrom, rangeTo);
    std::vector<double> Row;

    if (d > 0) {
        for (int i = 0; i < K; ++i) {
            for (int j = 0; j < d; ++j) {
                // Todo check if it is right.
                Row.push_back(distribution(generator));
            }
            G.push_back(Row);
            Row.clear();
        }
    }
}

#endif //ADS_PROJECT1_PROJECTION_H
