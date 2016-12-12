#include "TownsTSP.h"
#include "TownsATSP.h"
#include <ctime>

int main()
{

    std::string fileName[] = {"berlin52.tsp", "ch130.tsp", "a280.tsp"};

    for(int j = 0; j < 3; j++){
        TownsTSP towns = *(new TownsTSP());
        towns.loadMap(fileName[j]);

        clock_t START, STOP;
        double t = 0;
        int reps = 50;
        int results = 0;
        int optCost = towns.getOptCost();

        for(int i=0; i<reps;i++){
            START = clock();
            towns.tabuSearch();
            STOP = clock();

            results += towns.routeCost(towns.solution);
            t += (double)(STOP-START)/CLOCKS_PER_SEC*1000;
            towns.resetSolution();
        }

        results = results/reps;
        t = t/reps;
        double mistake = (double)(results - optCost)/(double)optCost;
        mistake *= 100;

        std::cout << fileName[j] << " best cost: " << optCost << " found: " << results << " time: " << t << " mistake " << mistake << "%\n";
    }

    return 0;
}