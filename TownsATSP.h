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

    void permuteRoute(int* route);
    void permuteRoute(int* route, TabuMatrix& tabu);
    void swapTowns(int* route, int a, int b);

public:
    int* solution;

    TownsATSP();
    ~TownsATSP();

    bool loadMap(std::string file_name);
    int getOptCost();
    int routeCost(int *route);
    void resetSolution();
    //algorithms
    void performSA();
    void deterministicTabu(int tt, int it);
    void randomNeighbourhoodTabu(int tt, int it, int nSize);
    void newGenetic(int generations, int popSize, int matingPopSize,int mutationProb);
    void invertMutate(int* chromosome, int genesNum);
    int rouletteSelection(int** population, double* costsTable, int popSize, int fitnessSum);
    void OXcrossover(int* parentA, int* parentB, int* childAB, int* childBA, int genesNum);
    void quicksort(double* costsArray, int** population, int left, int right);
    int getSize(){ return this->map_dim;}
};


#endif //TRAVELLINGSALESMANPROBLEMALGORITHMS_TOWNSATSP_H
