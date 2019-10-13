//
// Created by ody on 13/10/19.
//

#include "../inc/LSH_HT.h"
#include "../inc/util.h"

LSH_HT::LSH_HT(int w, int d, int k = 4, int r=1000):w(w),k(k),d(d),r(r){

    for (int j = 0; j < k; ++j) {
        hList.push_back(new FunctionH (4*r,d, k));
    }
}

LSH_HT::~LSH_HT(){
    typedef std::list <FunctionH *>::iterator functionHit;
    functionHit itS = hList.begin();
    functionHit itE = hList.end();
    functionHit it;
    for ( it = itS; it!=itE; ++it) {
        delete *it;
    }
}

int LSH_HT::calculateG(const Point * p) {
    /* Calculate the g from the k Hi */
    std::vector<int> hValues;
    int g = 0;
    //Calculate the values of each h
    for(auto h: hList){
        hValues.push_back(h->calculatePoint(p));
    }
    g = hValues[0];
    for (int i = 1; i < k; ++i) {
        g = (g << k) | hValues[i]; //concatenate the k bits of all Hi
    }
    return g;
}

void LSH_HT::addPoint(const Point * p) {/*Add Point to the hashtable */ ht[calculateG(p)] = p; }

std::list<const Point *> LSH_HT::getPoint(const Point * p, int radius, int topLimit) {
    /* Gather radius values from calculated g Key and return them in a list */
    int g = 0, i ;
    int limit = radius * topLimit; // multiply because if we want 5 neighbors we should get 5*topLimit
    std::list <const Point *> pList;
    typedef std::unordered_map<int, const Point *>::iterator umapIt;
    g = calculateG(p);
    // It returns a pair representing the range of elements with key equal to g
    std::pair<umapIt , umapIt > result = ht.equal_range(g);
    // Iterate over the range
    umapIt it;
    // Append to the list all the points between required range
    for(i = 0, it = result.first; (i< limit) && (it!=result.second) ; i++, it++){
        pList.push_back(it->second);
    }
    return pList;
}

LSH::LSH(int w, int d, int k = 4, int L = 5, int r = 1000):L(L) {
    /* Create the k Hi functions in a list */
    for (int j = 0; j < L; ++j) {
        htList.push_back(new LSH_HT (w, d, k, r));
    }
}

void LSH::addPoint(const Point * p) {
    /* Add p to all L hastables */
    for( auto ht : htList){
        ht->addPoint(p);
    }
}

std::list<const qPoint *> LSH::queryPoint(const Point * p, int radius, int topLimit) const{
    std::list<const Point * > pList;
    typedef std::list<const qPoint *> qpList;
    typedef qpList::iterator qpIt;
    qpList queryPointList;
    qpList distanceList;
    qpList returnList;
    int j;

    for (auto ht: htList){
        for (auto point : ht->getPoint(p,radius, topLimit)){
            // iterate through all returned points and append them in a list.
            queryPointList.push_back( new qPoint(point->getName(), manhattanDistance<int,std::vector<int>>(point->getList(), p->getList())));
        }
    }

    distanceList.sort(ComparatorqPoint());

    qpIt itS = distanceList.begin();
    qpIt it;
    qpIt itE = distanceList.end();
    for (j = 0, it = itS; (j < radius) && (it!=itE); ++j, ++it) {
        returnList.push_back(*it);
    }
}
