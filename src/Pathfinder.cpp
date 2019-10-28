//
// Created by user on 10/21/19.
//
#include "../inc/PathFinder.h"
#include<iomanip>
#include <cmath>

void div2(std::list<int> &I,int N, int cur, bool max = true);
PathFinder::PathFinder(int n, int m, unsigned int w) : N(n), M(m), initW(w){

    w = max<int>(w, abs(n - m));  // adapt window size (*)
}
PathFinder::PathFinder(int n, int m, unsigned int w, int k) : N(n), M(m),  initW(w), w(w){

    int flagGoOneMoreJ = 0;
    w = max<int>(w, abs(n - m));  // adapt window size (*)
    switch(w){
        case 11:
        {
            MainDActions.push_back({0,1});
            break;
        }
        case 1:
        {
            MainDActions.push_back({1,0});
            break;
        }
        case 2:
        {
            MainDActions.push_back({1,0});
            MainDActions.push_back({0,1});
            break;
        }
        case 3:
        {
            MainDActions.push_back({1,0});
            MainDActions.push_back({0,1});
            MainDActions.push_back({-1,0});
            break;
        }
        case 4:
        {
//            {{1, 0}, {-1,0},{0,1},{0,-1}}
            MainDActions.push_back({1,0});
            MainDActions.push_back({-1,0});
            MainDActions.push_back({0,1});
            MainDActions.push_back({0,-1});
            break;
        }
        case 5:
        {
//            {{1, 0}, {-1,0},{0,1},{0,-1}}
            MainDActions.push_back({1,0});
            MainDActions.push_back({-1,0});
            MainDActions.push_back({0,1});
            MainDActions.push_back({0,-1});

            MainDActions.push_back({1,-1});
            break;
        }
        default:
        {
//            {{1, 0}, {-1,0},{0,1},{0,-1}}
            MainDActions.push_back({1,0});
            MainDActions.push_back({-1,0});
            MainDActions.push_back({0,1});
            MainDActions.push_back({0,-1});
            MainDActions.push_back({1,-1});
            MainDActions.push_back({-1,1});
            break;
        }

    }
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
//    if (n-m  < 0) {
//        // TODO HAVE TEST IT , but not sure yet.
//        for (int i = 0; i < N; ++i) {
//            for (int j = 0; j < M; ++j) { // on flagGoOneMoreJ more js are on J.
//                // check 5,7 to see how it works
//                if (i == j && flagGoOneMoreJ == 0) {MainD.push_back({i,j});;
//                    // if N is odd when on middle change the flag value to 1
//                    if(N%2 == 1 && N/2 == i){ flagGoOneMoreJ = 1;}
//                    MainD.push_back({i, j+1});  // add 2nd j to MainD
//                    // if flag is on do the j+2 MainD
//                    if (flagGoOneMoreJ == 1)
//                        MainD.push_back({i, j+2});
//                }
//                // if flag is on do the j+1,j+2 MainD
//                if (i == j && flagGoOneMoreJ == 1) {
//                    MainD.push_back({i, j+1});
//                    MainD.push_back({i, j+2});
//                }
//            }
//        }
//    }
//    if (n-m  > 0) {
//        for (int i = 0; i < N; ++i) {
//            for (int j = 0; j < M; ++j) {
//                // TODO HAVE TESTed IT, but not 100 % sure...
//                // check 7,5 to see how it works
//                // on flagGoOneMoreJ more js are on I.
//                if (i == j && flagGoOneMoreJ == 0) {MainD.push_back({i,j});;
//                    MainD.push_back({i+1, j});
//                    // if M is odd when on middle change the flag value to 1
//                    if(M%2 == 1 && M/2 == j){ flagGoOneMoreJ = 1;}
//                    // if flag is on do the j+2 MainD
//                    if (flagGoOneMoreJ == 1)
//                        MainD.push_back({i+1, j});
//                }
//                // if flag is on do the j+1,j+2 MainD
//                if (i == j && flagGoOneMoreJ == 1) {
//                    MainD.push_back({i+1, j});
//                    MainD.push_back({i+2, j});
//                }
//            }
//        }
//    }
    if (n!=m){

        Paths paths =  FindPaths2(0,0);
        for (auto path : paths){
            auto p = path.back();
            if( p[0] == N-1 && p[1] == M-1)
                Fullpaths.push_back(path);
        }
        int tmp  , meanR, meanC,plusR, plusC;
        if(n<m){
            tmp = m/n;
            if (n*tmp == m){
                meanR = tmp;
                plusR = 0;
            }
            else{
                meanR = tmp;
                plusR = n -meanR;
            }
            tmp = ceil((double)N/M);
//            tmp = N/M;
            if (M*tmp == M){
                meanC = tmp;
                plusC = 0;
            }
            else{
                meanC = tmp;
                plusC = m - meanC;
            }

        }
        if(n>m){
            tmp = N/M;
            if (M*tmp == N){
                meanC = tmp;
                plusC = 0;
            }
            else{
                meanC = tmp;
                plusC = m - meanC;
            }
            tmp = ceil((double)M/N);
            if (N*tmp == N){
                meanR = tmp;
                plusR = 0;
            }
            else{
                meanR = tmp;
                plusR = n -meanR;
            }
        }
        int i, j;
        std::list<std::tuple<int,Path>> fps;
        // find the best path
        Path ps;
        int tempPlusR = plusR, tempPlusC = plusC;
        bool flagBreak = false;
        for (auto p : Fullpaths){
            std::vector<std::vector<int>> I(N, std::vector<int> (1,0));
            std::vector<std::vector<int>> J(M, std::vector<int> (1,0));
            tempPlusR = plusR, tempPlusC = plusC;
            flagBreak = false;
            ps = p;
            for( auto points : p){
                I[points[0]][0] += 1;
                i = I[points[0]][0];
                if (i > meanR){
                    if (tempPlusR == 0) { flagBreak = true; break;} // dont select this path.
                    else tempPlusR--;
                }

                J[points[0]][0] += 1;
                j = J[points[0]][0];
                if (j > meanC){
                    if (tempPlusC == 0) {flagBreak = true; break;} // dont select this path.
                    else tempPlusC--;
                }
//                for (int l = 0; l < N; ++l) {
//                    for (int i1 = 0; i1 < M; ++i1) {
//                        i = I[l][0];
//                        if (i > meanR){
//                            if (tempPlusR == 0) break; // dont select this path.
//                            else tempPlusR--;
//                        }
//                    }
//                }
//                if (J[points[0]][0] > meanC  && tempPlusC == 0) break;

            }
            if(!flagBreak){
                MainD = ps;
                std:: cerr << " found the best path";
                for (auto t : ps) {
                    std::cout << "(" << t[0] << ", " << t[1] << "), ";
                }
                break;
            }
//            fps.push_back(std::make_tuple(p.size(),p));
        }
//        fps.sort(TupleLess<0>());


//    }
//        std::list<int> I, J;
//        //start calculation of diagonal
//        int prevI = 0, curI = n/2, tempI, tempJ, prevJ = 0,curJ = m/2;
//        int i, j=m;
//        div2(I, N, N);
//        div2(J, M, M);
//        I.sort();
//        J.sort();
//        for (i = 0; i < N; ++i) {
//            for (j = 0; j < M; ++j) {
//                    MainD.push_back({i, j});;
//                    MainD.push_back({i + 1, j});
//            }
//        }

//        while(curI>0){
//            // now im in the middle must split to front and back.
//            //front
//            if(prevI != 0){I.push_front(prevI  - curI/2);}
//            else{ I.push_back((N - curI)/2 + curI);}
//            I.push_front(curI);
//            prevI  = curI;
//            curI = curI/2;
//        }
//        while(curJ>0){
//            if(prevJ != 0){J.push_front(prevJ  - curJ/2);}
//            else{ J.push_back((M - curJ)/2 + curJ);}
//            J.push_front(curJ);
//            prevJ  = curJ;
//            curJ = curJ/2;
//        }
//        std:: cout << std::endl;
//        // build the diagonal now.
//
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

void div2(std::list<int> &I,int N, int cur, bool max ){

    int prevI = 0, curI = cur/2, tempI;
    while(curI>0){
        // now im in the middle must split to front and back.
        //front
        if(prevI != 0){I.push_front(prevI  - curI/2);
        div2(I, N, curI, false);
        }
        // back
        else{ I.push_back((N - curI)/2 + curI);}
//        if(max){ I.push_back((N - curI)/2 + curI);}
        div2(I, N, curI, true);
//        I.push_front(curI);
        prevI  = curI;
        curI = curI/2;
    }
    if(curI != 0)
        I.push_front(curI);
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
            if (i==j)
                flag2 = 1;
            for(auto in : MainD){
                if ((in[0] == i && in[1] == j) || (i==j) )
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
    cout << endl;
}

std::list<std::list<std::vector<int>>> PathFinder::RelevantPaths(){
    /**
     * @brief Create Relevant Paths
     * @return full paths in a list of Paths, Path=list<vector<int>> , vector stores
     * i,j
     */
    w = max<int>(initW, abs(N - M));  // adapt window size (*)
     setMinMaxCol(0);
    Paths paths =  FindPaths2(0,0);
    TotalPathsSize += paths.size();

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
//    PrintTable();
    if (FullPaths.size() == 0) {std::cerr << N << M << "error no paths" << std::endl;}
    Paths fp1;
    if(!FullPaths.empty())
        fp1.push_back(FullPaths.front());
//    return FullPaths;
    return fp1;//todo fix this
}

 std::list<std::list<std::vector<int>>> PathFinder::FindPaths(int i, int j){
    Paths paths;
    int stepRow, stepColumn;

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

std::list<std::list<std::vector<int>>> PathFinder::FindPaths2(int i, int j){
    Paths paths;
    int stepRow, stepColumn;
    for (auto action:Actions){              // for every action do:
        stepRow = i + action[0];            // calculate new i
        if(action[0] == 1){setMinMaxCol(stepRow);} // set limits on J
        stepColumn = j + action[1];         // calculate new j
//        for (j = max(1, i - w); j < min(m, i + w); ++j) {               // For all columns

//        if(i == 2 && j == 5) {
//            std:: cout << std::endl;
//        }

        if (isValid2(stepRow, stepColumn)){
            paths.push_back(Path {{i,j},{stepRow, stepColumn}});
            for (auto path: FindPaths2(stepRow, stepColumn)){
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

bool PathFinder::isValid2(int i, int j) {
    return (i >= 0) && (i < N) && j < M;
}
    //(j >= minCol) && ( j < maxCol);}

void PathFinder::setMinMaxCol(int i){

//    if(M > 1)
        minCol = max<int>(0, i - w + 1);
        if (minCol >= M) minCol = 0;
//    else minCol = 1;
    maxCol = min<int>(M, i + w);
//    if (maxCol > M) minCol = 0;
}
