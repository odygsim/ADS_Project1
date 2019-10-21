//
// Created by user on 10/21/19.
//

#ifndef ADS_PROJECT1_PATHFINDER_H
#define ADS_PROJECT1_PATHFINDER_H

#include "util.h"


class PathFinder{
    int N, M; // rows, columns, window

    typedef std::vector<int> Vector; // store x, y
    typedef std::list<Vector> Path; // store list of a path
    typedef std::list<Path> Paths;  // store many paths
    std::list<std::vector<int>> PrintThesePaths;    // these paths are printed
    std::list<std::vector<int>> MainD;  // store main diagonal elements
    std::vector<std::vector<int>> ArrayJLimits;     // store i, jmin, jmax
    // go N, E, NE
    std::list<std::vector<int>> Actions{{1, 0}, {0,1},{1,1}};
    // N, S, E, W , NW, SW
    std::list<std::vector<int>> MainDActions{{1, 0}, {-1,0},{0,1},{0,-1}, {1,-1}, {-1,1}};
    int red = 31, green = 32;


public:
    PathFinder(int n, int m);
    void PrintTable();
    Paths RelevantPaths();
    Paths FindPaths(int i, int j);
    bool isValid(int i, int j);
    void Color(int i );

};

#endif //ADS_PROJECT1_PATHFINDER_H
