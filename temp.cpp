
#include "TownsTSP.h"
#include "TownsATSP.h"
#include <iostream>

// Created by piotrek on 13.01.17.
//

int ain(){

//    TownsATSP* instance = new TownsATSP();
//    TownsTSP* instance2 = new TownsTSP();
//    instance->loadMap("ft70.atsp");
//    instance2->loadMap("berlin52.tsp");
//
//    double initial = instance->routeCost(instance->solution);
//    instance->genetic(200,5000,0.1,SelectionStrategy::TOURNAMENT, CrossoverStrategy::RANDOM);
//    double cost = instance->routeCost(instance->solution);
//    int best = instance->getOptCost();
//    double mistake = 100*(cost - best) / best;
//
//
//    double initial2 = instance2->routeCost(instance2->solution);
//    instance2->genetic(100,5000,0.1,SelectionStrategy::TOURNAMENT, CrossoverStrategy::RANDOM);
//    double cost2 = instance2->routeCost(instance2->solution);
//    int best2 = instance2->getOptCost();
//    double mistake2 = 100*(cost2 - best2) / best2;
//    std::cout << best << " " <<initial << " " << cost <<" " << mistake << std::endl;
//    std::cout << best2 << " " <<initial2 << " " << cost2 <<" " << mistake2 << std::endl;
//
//    return 0;

    std::string fileNameTSP[] = {"berlin52.tsp", "ch130.tsp", "a280.tsp"};
    std::string fileNameATSP[] = {"ftv35.atsp", "ft70.atsp", "ftv170.atsp"};
    int whichInstance = 2;

    double resultsTSP = 0;
    double mistakeTSP = 0;
    double ATSPresults = 0;
    double ATSPmistake = 0;

    double mistakes_TSP[11];
    double timesTSP[11];
    double ATSP_mistakes[11];
    double ATSP_times[11];

    int generationsNum[] = {25,50,100,200,400};
    int population_size[] = {100,500,1000,1500,2000};
    int gen = 5;
    int pop = 5;

    clock_t START, STOP;
    double t_TSP=0;
    double t_ATSP = 0;
    int tests = 10;

    std::fstream fileTSP;
    std::fstream ATSPfile;
    std::string nameTSP = "a2800_different_pop_and_gen.csv";
    std::string ATSPname = "ftv170_different_pop_and_gen.csv";

    fileTSP.open(nameTSP, std::ios::out);
    ATSPfile.open(ATSPname, std::ios::out);

    for(int i=0; i<pop; i++){
        fileTSP << population_size[i] << ";";
        ATSPfile << population_size[i] << ";";
    }

    fileTSP << std::endl;
    ATSPfile << std::endl;

    for(int k=0; k<gen; k++)
    {
        std::cout<<"k="<<k<<std::endl;

        for(int j=0; j<pop; j++) {
            TownsATSP* ATSPinstance = new TownsATSP();
            ATSPinstance->loadMap(fileNameATSP[whichInstance]);

            TownsTSP* instanceTSP = new TownsTSP();
            instanceTSP->loadMap(fileNameTSP[whichInstance]);

            int optCostTSP = instanceTSP->getOptCost();
            int ATSPoptCost = ATSPinstance->getOptCost();

            std::cout<<"generations num: " << generationsNum[k] <<std::endl;
            for (int i = 0; i < tests; i++) {
                std::cout<<"test " << i << std::endl;
                START = clock();
                instanceTSP->newGenetic(generationsNum[k], population_size[j], (int)(0.25*population_size[j]),5);
                STOP = clock();

                t_TSP += (double) (STOP - START) / CLOCKS_PER_SEC * 1000;


                resultsTSP = instanceTSP->routeCost(instanceTSP->solution);
                mistakeTSP += (resultsTSP - optCostTSP) / (double) optCostTSP;
                instanceTSP->resetSolution();

                START = clock();
                ATSPinstance->newGenetic(generationsNum[k], population_size[j], (int)(0.25*population_size[j]),5);
                STOP = clock();

                t_ATSP += (double) (STOP - START) / CLOCKS_PER_SEC * 1000;

                ATSPresults = ATSPinstance->routeCost(ATSPinstance->solution);
                ATSPmistake += (ATSPresults - ATSPoptCost) / (double) ATSPoptCost;
                ATSPinstance->resetSolution();

            }
            std::cout<<"tests done" <<std::endl;

            mistakeTSP *= 100;
            mistakeTSP = mistakeTSP/tests;

            ATSPmistake *= 100;
            ATSPmistake = ATSPmistake/tests;

            t_TSP = t_TSP/tests;
            t_ATSP = t_ATSP/tests;

            mistakes_TSP[j] = mistakeTSP;
            timesTSP[j] = t_TSP;

            ATSP_mistakes[j] = ATSPmistake;
            ATSP_times[j] = t_ATSP;

            mistakeTSP = 0;
            t_TSP = 0;

            ATSPmistake = 0;
            t_ATSP = 0;

            delete instanceTSP;
            delete ATSPinstance;
        }

        for(int i=0; i<pop; i++){
            fileTSP << mistakes_TSP[i] << ";";
            ATSPfile << ATSP_mistakes[i] << ";";
        }

        fileTSP << std::endl;
        ATSPfile << std::endl;

        for(int i=0; i<pop; i++){
            fileTSP << timesTSP[i] << ";";
            ATSPfile << ATSP_times[i] << ";";
        }

        fileTSP << std::endl;
        ATSPfile << std::endl;
    }

    fileTSP.close();
    ATSPfile.close();
    return 0;
}
