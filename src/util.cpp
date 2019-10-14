// this file will contain general used utilities
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <list>
#include <fstream>
#include "../inc/Point.h"
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

double manhattanDistance(std::vector<int> point1, std::vector<int> point2) {
    long sum = 0;

    std::vector<int>::iterator b1 = point1.begin();
    std::vector<int>::iterator b2 = point2.begin();
    std::vector<int>::iterator e1 = point1.end();
    std::vector<int>::iterator e2 = point2.end();
    std::vector<int>::iterator it1;
    std::vector<int>::iterator it2;
    for (it1 = b1, it2 = b2; (it1 != e1) && (it2 != e2); ++it1, ++it2) {
//        std::cout << "" << it1. << " " << it2 << " " << std::endl;
        sum += std::abs(point1[*it1] - point1[*it2]);
    }


//    for (unsigned long i = 0; i < point1.size(); ++i)
//        sum += abs(point1.at(i) - point2.at(i));

    return (double) sum;
}

std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}


std::vector<int> splitInt(const std::string &s, char delimiter) {
    std::vector<int> tokens;
    std::string token;
    size_t *pEnd = nullptr;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(stoi(token));
    }
    return tokens;
}

Point *splitToPoint(const std::string &s, char delimiter) {
    std::vector<int> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    int i = 0;
    std::string name;
    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            if (i == 0)
                name = token;
            else
                tokens.push_back(stoi(token));
        }
        i += 1;
    }
    return new Point(name, tokens);
}

struct ComparatorqPoint {
//    bool operator(const qPoint * qPointObjA, const qPoint * qPointObjB ) const {
//        return qPointObjA->getDistance() < qPointObjB->getDistance();
//    }
//    bool operator() (qPoint const & qPointObjA, qPoint const & qPointObjB ) {
//        return qPointObjA.getDistance() < qPointObjB.getDistance();
//    }
    bool operator()(const qPoint *a,const  qPoint *b) {
        return a->getDistance() < b->getDistance();
    }
};

const qPoint * exactKNN(std::vector<const Point *> dataList, const Point *queryPoint) {
    std::list<const qPoint *> distanceList;

    for (unsigned long i = 0; i < dataList.size(); ++i)
        distanceList.push_back(
                new qPoint(dataList[i]->getName(), manhattanDistance(dataList[i]->getList(), queryPoint->getList())));
    distanceList.sort(ComparatorqPoint());

    return distanceList.front();
}

std::vector<const Point *> copyData(const std::string& fileName) {
    std::vector<const Point *> dataList;
    std::ifstream file;
    std::string tmp;

    // Check filenames
    if (file.fail()) {
        std::cerr << "Error: Wrong filename / notFound" << std::endl;
        exit(1) ;
    }
    // Test if file can be opened
    file.open(fileName);
    if (!file) {
        std::cerr << "Unable to open file : " << fileName << std::endl;
        exit(1);
    }
    // Iterate over input file and store each Point's dimension data in a vector
    while (!file.eof()) {
        getline(file, tmp, '\r'); // remove /r
        if (tmp[0] == '\n')
            tmp.erase(tmp.begin());
        if (!tmp.empty())
            dataList.push_back(splitToPoint(tmp, ' '));
    }
    file.close();
    return dataList;
}


void print_cube_usage() {
    fprintf(stderr, "Usage: cube -d <input file> -q <query file> k <int> -M <int> -probes <int> -o <output file>\n");
}

int readHypercubeParameters(int argc, char** argv,
        std::string &inputFile, std::string &queryFile, std::string &outputFile,
        int &k, int &M, int &probes){

    extern char *optarg;
    int opt=0;

    //Set default parameters values
    k=3; M=10; probes=2;

    //Specify expected options
    static struct option cube_options[] = {
            {"d", required_argument, 0, 'd' },
            {"q", required_argument, 0, 'q' },
            {"o", required_argument, 0, 'o' },
            {"k", required_argument, 0,  'k' },
            {"M", required_argument, 0,  'M' },
            {"probes", required_argument,0,'p' }
    };

    int longIndex = 0;
    int dflag = 0, qflag = 0, oflag = 0;

    // Get options given
    while ((opt = getopt_long_only(argc, argv,"",
                                   cube_options, &longIndex )) != -1) {
        switch (opt) {
            case 'd' :
                dflag = 1;
                inputFile = optarg;
                break;
            case 'q' :
                qflag = 1;
                queryFile = optarg;
                break;
            case 'o' :
                oflag = 1;
                outputFile = optarg;
                break;
            case 'k' :
                k = atoi(optarg);
                break;
            case 'M' :
                M = atoi(optarg);
                break;
            case 'p' :
                probes =  atoi(optarg);
                break;
            default: print_cube_usage();
                exit(EXIT_FAILURE);
        }
    }

    //  Check if necessary options have been provided
    if (dflag == 0) {
        fprintf(stderr, "cube: missing -d option\n");
        print_cube_usage();
        exit(EXIT_FAILURE);
    } else if (qflag == 0 ){
        fprintf(stderr, "cube: missing -q option\n");
        print_cube_usage();
        exit(EXIT_FAILURE);
    } else if (oflag == 0 ){
        fprintf(stderr, "cube: missing -o option\n");
        print_cube_usage();
        exit(EXIT_FAILURE);
    }

    return 0;
}