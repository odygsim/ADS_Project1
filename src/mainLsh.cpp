// This file will be the main of lsh

#include<iostream>
#include <getopt.h>
#include<cstring>
#include <fstream>
//#include <stdio.h>
//#include <unistd.h>
//#include <vector>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator
//#include <cstring>
#include<cstdlib>
#include "../inc/Point.h"
#include "../inc/util.h"


int main(int argc, char ** argv)
{
    using namespace std;

    int L=0, k=0;
    char delim = ' ', *pEnd;
    char delimN = '\n';
    const char *arg;
    string oFileName, iFileName, qFileName, tmp;
    ifstream oFile, iFile, qFile;
    vector <string> words;
    vector <Point * > dataList;
    string name;
    int kk = -2;
    //Read args
    if (argc != 12) {
        fprintf(stderr, "Usage : %s -d <input file> -q <query file> -k <int> -L <int> -o <output file>\n", argv[0]);
        return 1;
    }
    while (--argc && argc > 4) {
        arg = *++argv;
        if (!strcmp(arg, "-o")) {
            if (argc > 1 && --argc)
                oFileName = *++argv;
        } else if (!strcmp(arg, "-L")) {
            L = (int) strtol(*++argv, &pEnd, 10);
        } else if (!strcmp(arg, "-k"))
            k = (int) strtol(*++argv, &pEnd, 10);
        else if (!strcmp(arg, "-q"))
            qFileName = *++argv;
        else if (!strcmp(arg, "-d"))
            iFileName = *++argv;
    }

    cout << "Running Instance with args " << iFileName << " " << qFileName << " " << k << " " << L << " " << oFileName << endl;
    // Check filenames
    if (iFile.fail())
    { cerr << "Error: Wrong filename / notFound" << endl;
        return 1;
    }
    if (qFile.fail())
    { cerr << "Error: Wrong filename / notFound" << endl;
        return 1;
    }
    if (oFile.fail())
    { cerr << "Error: Wrong filename / notFound" << endl;
        return 1;
    }
    // Test if file can be opened
    iFile.open(iFileName);
    if(!iFile){
        cerr << "Unable to open file : " << iFileName << endl;
        return 1;
    }
    int i = 0;
    while (!iFile.eof()){
        getline(iFile, tmp, '\r');
        dataList.push_back(splitToPoint(tmp,' '));
        i += 1;

    }

    iFile.close();

    return 0;
}
