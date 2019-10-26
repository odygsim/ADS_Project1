//
// Created by user on 10/21/19.
//

#ifndef ADS_PROJECT1_PATHFINDER_H
#define ADS_PROJECT1_PATHFINDER_H

#include "util.h"

typedef std::vector<int> Vector; // store x, y
typedef std::list<Vector> Path; // store list of a path
typedef std::list<Path> Paths;  // store many paths

class PathFinder{
    int N, M, w , initW, minCol, maxCol, TotalPathsSize; // rows, columns, window

//    typedef std::vector<int> Vector; // store x, y
//    typedef std::list<Vector> Path; // store list of a path
//    typedef std::list<Path> Paths;  // store many paths
    std::list<Vector> PrintThesePaths;    // these paths are printed
    std::list<Vector> MainD;  // store main diagonal elements
    std::vector<Vector> ArrayJLimits;     // store i, jmin, jmax
    // go N, E, NE
    // N, S, E, W , NE, SE
//    std::list<Vector> Actions{{1, 0},{-1,0},{0,1},{0,-1},{-1,1},{1,1}};
//    std::list<Vector> Actions{{0,1},{-1,1},{1,1}};
    std::list<Vector> Actions{{0,1},{1,0},{1,1}};
//    std::list<Vector> Actions{{0,1},{1,0}};
//    {1, 0}, {0,1},{1,1}
    // N, S, E, W , NW, SW
//    std::list<std::vector<int>> MainDActions{{1, 0}, {-1,0},{0,1},{0,-1},{1,1}{-1,-1}};
    // N, S, E, W , NE, SE
//    std::list<std::vector<int>> MainDActions{{1, 0},{-1,0},{0,1},{0,-1},{-1,1}{1,1}};
    std::list<std::vector<int>> MainDActions;
    Paths Fullpaths;
    int red = 31, green = 32;


public:
    PathFinder(int n, int m, unsigned int w=1);
    PathFinder(int n, int m, unsigned int w, int k);
//    PathFinder(int n, int m);
    ~PathFinder(){};
    void PrintTable();
    int getSize(){return TotalPathsSize;}
    void setNM(int n,int m){N=n;M=m;}
    void setW(int w){ w = w;}
    Paths RelevantPaths();
    Paths FindPaths(int i, int j);
    Paths FindPaths2(int i, int j);
    bool isValid(int i, int j);
    void Color(int i );
    void setMinMaxCol(int i);
    bool isValid2(int i, int j);

};

#endif //ADS_PROJECT1_PATHFINDER_H
