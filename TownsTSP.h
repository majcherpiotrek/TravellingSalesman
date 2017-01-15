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
#include "Specimen.h"

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
    void permuteRoute(int* route);
    void permuteRoute(int* route, TabuMatrix& tabu);
    void swapTowns(int* route, int a, int b);
public:
    enum SelectionStrategy{
        TOURNAMENT, ACCEPTREJECT, ROULETTE
    };
    int* solution;

    TownsTSP();
    ~TownsTSP();

    bool loadMap(std::string file_name);
    int getOptCost();
    double routeCost(int *route);
    void resetSolution();
    int getSize(){return this->map_dim;}
    //algoritms
    void performSA();
    void deterministicTabu(int tt, int it);
    void randomNeighbourhoodTabu(int tt, int it, int nSize);
    void genetic(int generations,int populationSize, double elitarismFactor, SelectionStrategy startegy);
    void generateRandomPopulation(Specimen** population, int populationSize);
    double getSpecimenRouteCost(Specimen *specimen, int *root);
    Specimen* tournament(Specimen** population,int populationSize, int tournamentMembers);
    Specimen* acceptReject(Specimen** population, int populationSize);
    Specimen* roulette(Specimen** population, int populationSize, int fitnessSum);
    void onePointCrossover(Specimen *parent1, Specimen *parent2, Specimen *child1, Specimen *child2, int genesNum);
    void twoPointCrossover(Specimen *parent1, Specimen *parent2, Specimen *child1, Specimen *child2, int genesNum);


};



#endif //TRAVELLINGSALESMANPROBLEMALGORITHMS_TOWNSTSP_H
