//
// Created by ody on 13/10/19.
//

#ifndef ADS_PROJECT1_LSH_HT_H
#define ADS_PROJECT1_LSH_HT_H


#include <list>
#include <unordered_map>
#include "FunctionH.h"


class LSH_HT {
    int w, k, d, r;
    std::unordered_map<int, const Point *> ht;
    std::list<FunctionH * > hList;
    int calculateG(const Point * p) ;

public:
    LSH_HT(int w, int k, int, int);
    ~LSH_HT();
    void fit();
    void addPoint(const Point *) ;
    std::list<const Point *> getPoint(const Point *, int, int);

};

class LSH {
    int L;
    std::list<LSH_HT *> htList;
    LSH(int, int, int, int, int);
public:
    void addPoint(const Point *) ;
    std::list<const qPoint *> queryPoint(const Point *, int radius, int) const;
};

#endif //ADS_PROJECT1_LSH_HT_H
