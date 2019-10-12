// this file will contain general used utilities
#include <iostream>
#include <cstdlib>
#include <sstream>
#include "../inc/Point.h"

double calculateManhattan(){};

std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}


std::vector< int> splitInt(const std::string &s, char delimiter) {
    std::vector<int> tokens;
    std::string token;
    size_t *pEnd = nullptr;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(stoi(token));
    }
    return tokens;
}

Point * splitToPoint(const std::string& s, char delimiter)
{
    std::vector<int> tokens;
    std::string token;
    size_t *pEnd = nullptr;
    std::istringstream tokenStream(s);
    int i = 0;
    std::string name;
    while (std::getline(tokenStream, token, delimiter))
    {
        std::cout << token << "\n" << std::flush ;
        if (i == 0)
            name = token;
        else
//            if (token == "23")
//                std::cerr << "er 23" << std::endl;
//            if ((token != " ") && (token != "\n") && (token!= "\r" ))
                tokens.push_back(stoi(token));
        i += 1;
    }
    return new Point(name,tokens);
}
