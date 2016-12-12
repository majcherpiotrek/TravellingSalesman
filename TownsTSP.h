//
// Created by piotrek on 01.12.16.
//

#ifndef TRAVELLINGSALESMANPROBLEMALGORITHMS_TOWNSTSP_H
#define TRAVELLINGSALESMANPROBLEMALGORITHMS_TOWNSTSP_H


#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <time.h>
#include <string.h>
#include <cstdlib>
#include <float.h>
#include "TabuQueue.h"
#include "TabuMatrix.h"

struct coords
{
    double x;
    double y;
};

class TownsTSP
{
private:

    const int L = 180;
    const double t_rate = 0.999;
    const std::string opt_solutions_file = "tsp.opt.tour";
    std::string problem_name;
    unsigned short int map_dim; // Liczba miast w instancji problemu
    coords* coords_list;
    double** towns_map;

    double computeDistance(coords A, coords B);
    int* makeNeighbourPermutation(int* basePermutation, int size, int moveBeg, int moveEnd);
    void permuteRoute(int* route);
    void swapTowns(int* route, int a, int b);
    double mval(int* solutionA, int* solutionB);
    void determineMaxMoves(TabuQueue::Move*& maxMoves, int* positions, int cityCount, TabuQueue& tabu);
public:
    int* solution;

    TownsTSP();
    ~TownsTSP();

    bool loadMap(std::string file_name);
    int getOptCost();

    double routeCost(int *route);

    void performSA();
    void tabuSearch();
    void resetSolution();
    void resetSolution(int* solution);
};



#endif //TRAVELLINGSALESMANPROBLEMALGORITHMS_TOWNSTSP_H
