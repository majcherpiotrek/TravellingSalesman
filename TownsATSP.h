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
#include <time.h>
#include <string.h>
#include <cstdlib>


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


public:
    int* solution;

    TownsATSP();
    ~TownsATSP();

    bool loadMap(std::string file_name);
    int getOptCost();

    int routeCost(int *route);

    void performSA();
    void tabuSearch(int iterations);
    void resetSolution();

};


#endif //TRAVELLINGSALESMANPROBLEMALGORITHMS_TOWNSATSP_H
