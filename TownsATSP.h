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
#include "Specimen.h"
#include "SelectionStrategy.h"
#include "CrossoverStrategy.h"

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
    void genetic(int generations,int populationSize, double elitarismFactor, SelectionStrategy startegy, CrossoverStrategy crossoverStrategy);
    void generateRandomPopulation(Specimen** population, int populationSize);
    double getSpecimenRouteCost(Specimen *specimen, int *root);
    Specimen* tournament(Specimen** population,int populationSize, int tournamentMembers);
    Specimen* roulette(Specimen** population, int populationSize, int fitnessSum);
    void onePointCrossover(Specimen *parent1, Specimen *parent2, Specimen *child1, Specimen *child2, int genesNum);
    void twoPointCrossover(Specimen *parent1, Specimen *parent2, Specimen *child1, Specimen *child2, int genesNum);
    void mutate(Specimen* specimen,int genesNum, int mutationProbability, int mutationsNum);
    void randomCrossover(Specimen *parent1, Specimen *parent2, Specimen *child1, Specimen *child2, int genesNum);

    int getSize(){ return this->map_dim;}
};


#endif //TRAVELLINGSALESMANPROBLEMALGORITHMS_TOWNSATSP_H
