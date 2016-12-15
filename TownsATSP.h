//
// Created by piotrek on 01.12.16.
//

#ifndef TRAVELLINGSALESMANPROBLEMALGORITHMS_TOWNSATSP_H
#define TRAVELLINGSALESMANPROBLEMALGORITHMS_TOWNSATSP_H


#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <float.h>
#include <time.h>
#include <string.h>
#include <cstdlib>
#include "TabuMatrix.h"
#include "TabuQueue.h"

class TownsATSP
{

private:

    const int L = 180;
    const double t_rate = 0.999;

    const std::string opt_solutions_file = "atsp.opt.tour";
    std::string problem_name;
    unsigned short int map_dim; // Liczba miast w instancji problemu
    int** towns_map;

    int* getNeighbourPermutation(int* basePermutation, int size);
    void permuteRoute(int* route);
    void permuteRoute(int* route, TabuMatrix& tabu);


public:
    int* solution;

    TownsATSP();
    ~TownsATSP();

    bool loadMap(std::string file_name);
    int getOptCost();

    int routeCost(int *route);

    void performSA();
    void deterministicTabu(int iterations);
    void randomNeighbourhoodTabu(int tt);
    void resetSolution();
    void swapTowns(int* route, int a, int b);
    void resetSolution(int* solution);
    int getSize(){ return this->map_dim;}
};


#endif //TRAVELLINGSALESMANPROBLEMALGORITHMS_TOWNSATSP_H
