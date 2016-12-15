#include "TownsTSP.h"
#include "TownsATSP.h"
#include <ctime>
#include <fstream>

int main()
{

    std::string fileName[] = {"berlin52.tsp", "ch130.tsp", "a280.tsp"};
    std::string fileNameATSP[] = {"ftv35.atsp", "ft70.atsp", "ftv170.atsp"};
    int whichInstance = 0;
        TownsTSP townsT = *(new TownsTSP());
        townsT.loadMap(fileName[whichInstance]);

        TownsATSP townsA = *new TownsATSP();
        townsA.loadMap(fileNameATSP[whichInstance]);

        double results = 0;
        double resultsA = 0;
        int optCost = townsT.getOptCost();
        int optCostA = townsA.getOptCost();

        double mistakes[1];
        double mistakesA[1];


        clock_t START, STOP;
        double tT, tA;

        for(int i = 0; i< 1; i++){


            START = clock();
            townsT.deterministicTabu(3 * townsT.getSize(), 100);
            STOP = clock();

            tT = (double)(STOP - START)/CLOCKS_PER_SEC*1000;

            START = clock();
            townsA.deterministicTabu((int)(3*townsA.getSize()));
            STOP = clock();

            tA = (double)(STOP - START)/CLOCKS_PER_SEC*1000;

            results = townsT.routeCost(townsT.solution);
            resultsA = townsA.routeCost(townsA.solution);


            townsT.resetSolution();
            townsA.resetSolution();

            double mistake = (results - optCost)/(double)optCost;
            double mistakeA = (resultsA - optCostA)/(double)optCostA;

            mistake *= 100;
            mistakeA *= 100;

            mistakes[i] = mistake;
            mistakesA[i] = mistakeA;
        }


    //std::fstream file;
    //std::string name = "test_results/test_tt_aspiration_berlin52.csv";
    //file.open(name, std::ios::out);
    //for(int i=0; i<100; i++)
      //  file << mistakes[i] << ";";
    //file.close();

    std::cout << "opt: " << optCost << " result: " << results << " mistake TSP: " << mistakes[0] << " time: " << tT << std::endl;
    std::cout << "opt: " << optCostA << " result: " << resultsA << " mistake ATSP : " <<mistakesA[0]<< " time: " << tA << std::endl;
    return 0;
}