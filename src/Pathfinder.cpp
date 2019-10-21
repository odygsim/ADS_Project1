//
// Created by user on 10/21/19.
//
#include "../inc/PathFinder.h"
#include<iomanip>

PathFinder::PathFinder(int n, int m) : N(n), M(m){

    int flagGoOneMoreJ = 0;
    //  init ArrayJLimit
    for (int l = 0; l < N; ++l) {
        // init the min j with int32 max
        ArrayJLimits.push_back({INT32_MAX, 0});      // init the max j with zero
    }
    if (n-m  == 0) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                if (i == j) MainD.push_back({i,j});
            }
        }
    }
    if (n-m  < 0) {
        // TODO HAVE TEST IT , but not sure yet.
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) { // on flagGoOneMoreJ more js are on J.
                // check 5,7 to see how it works
                if (i == j && flagGoOneMoreJ == 0) {MainD.push_back({i,j});;
                    // if N is odd when on middle change the flag value to 1
                    if(N%2 == 1 && N/2 == i){ flagGoOneMoreJ = 1;}
                    MainD.push_back({i, j+1});  // add 2nd j to MainD
                    // if flag is on do the j+2 MainD
                    if (flagGoOneMoreJ == 1)
                        MainD.push_back({i, j+2});
                }
                // if flag is on do the j+1,j+2 MainD
                if (i == j && flagGoOneMoreJ == 1) {
                    MainD.push_back({i, j+1});
                    MainD.push_back({i, j+2});
                }
            }
        }
    }
    if (n-m  > 0) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                // TODO HAVE TESTed IT, but not 100 % sure...
                // check 7,5 to see how it works
                // on flagGoOneMoreJ more js are on I.
                if (i == j && flagGoOneMoreJ == 0) {MainD.push_back({i,j});;
                    MainD.push_back({i+1, j});
                    // if M is odd when on middle change the flag value to 1
                    if(M%2 == 1 && M/2 == j){ flagGoOneMoreJ = 1;}
                    // if flag is on do the j+2 MainD
                    if (flagGoOneMoreJ == 1)
                        MainD.push_back({i+1, j});
                }
                // if flag is on do the j+1,j+2 MainD
                if (i == j && flagGoOneMoreJ == 1) {
                    MainD.push_back({i+1, j});
                    MainD.push_back({i+2, j});
                }
            }
        }
    }
    int ik, jk;
    std::list<std::vector<int>> PossibleMovesList;
    // find all possible moves
    for(auto itemRC:MainD){ // for all points from diagonal
        for(auto k : MainDActions){ // for all action N, S etc
            ik = k[0] + itemRC[0];
            jk = k[1] + itemRC[1];
            if (ik>=0 && ik < N && jk>=0 && jk <=M)
                PossibleMovesList.push_back({ik, jk});
        }
    }
    // Now find the limits of j
    for (auto points: PossibleMovesList){
        for (int i = 0; i < N; ++i) { // for all i
            if (points[0] == i){        // if i == pointsp[0] from possible moves
                ArrayJLimits[i][0] = min<int>(points[1], ArrayJLimits[i][0]);
                ArrayJLimits[i][1] = max<int>(points[1], ArrayJLimits[i][1]);
            }
        }
    }
}
void PathFinder::Color(int i ){
    std::cout << "\033[" << i << "m";
}

void PathFinder::PrintTable(){
    using namespace std;
    int flag;
    for (int i = N-1; i > -1; --i) {
        //print line
        for (int j = 0; j < M; ++j) {
            Color(39);
            cout << "|" << setw(2) << " ---" << setw(2);
        }
        cout << "|" << endl;
        for (int j = 0; j < M; ++j) {
            flag = 0;int flag2 = 0;
            for(auto in : PrintThesePaths){
                if (in[0] == i && in[1] == j)
                    flag =1;
            }
            for(auto in : MainD){
                if (in[0] == i && in[1] == j)
                    flag2 = 1;
            }
            if (flag2 == 1){
                Color(39);
                cout << '|' ;
                Color(green);
                cout << setw(2) << i<< ",";
                Color(green);
                cout << j << setw(2);
            } else if (flag == 1){
                Color(39);
                cout << '|' ;
                Color(red);
                cout << setw(2) << i<< ",";
                Color(red);
                cout << j << setw(2);
            }
            else {
                Color(39);
                cout << '|' << setw(2) << i << "," << j << setw(2);
            }
        }
        Color(39);
        cout << "|" << endl;
    }
}

std::list<std::list<std::vector<int>>> PathFinder::RelevantPaths(){
    /**
     * @brief Create Relevant Paths
     * @return full paths in a list of Paths, Path=list<vector<int>> , vector stores
     * i,j
     */
    Paths paths =  FindPaths(0,0);
    Paths FullPaths;
    for (auto path : paths){
        auto p = path.back();
        if( p[0] == N-1 && p[1] == M-1)
            FullPaths.push_back(path);
    }
    int flag = 0;
    for(const auto& path : paths){
        for (auto t : path){
            flag = 0;
            for(auto in: PrintThesePaths){
                if (in[0] == t[0] && in[1] == t[1])
                    flag = 1;
            }
            if(flag == 0)
                PrintThesePaths.push_back({t});
        }
    }
    return FullPaths;
}

 std::list<std::list<std::vector<int>>> PathFinder::FindPaths(int i, int j){
    Paths paths;
    int stepRow, stepColumn, stepStartColumn, stepEndColumn;
    for (auto action:Actions){              // for every action do:
        stepRow = i + action[0];            // calculate new i
        stepColumn = j + action[1];         // calculate new j

        if (isValid(stepRow, stepColumn)){
            paths.push_back(Path {{i,j},{stepRow, stepColumn}});
            for (auto path: FindPaths(stepRow, stepColumn)){
                Path step {{i,j}}; // create a list with current step
                step.splice(step.end(),path);   // concatenate the two lists with i,j and the rest path
                paths.push_back(step);
            }
        }
    }
    return paths;
}

bool PathFinder::isValid(int i, int j){
    return (i<N && (j >= ArrayJLimits[i][0]) && ( j <= ArrayJLimits[i][1]));}


