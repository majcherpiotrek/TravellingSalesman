#include "TownsTSP.h"
#include "TownsATSP.h"
#include <ctime>
#include <fstream>

int main()
{

    std::string fileName[] = {"berlin52.tsp", "ch130.tsp", "a280.tsp"};
    int whichInstance = 1;
        TownsTSP towns = *(new TownsTSP());
        towns.loadMap(fileName[whichInstance]);



        double results = 0;
        int optCost = towns.getOptCost();

        double mistakes[100];
        for(int i = 0; i< 100; i++){


                towns.tabuSearch(i);

                results = towns.routeCost(towns.solution);
                towns.resetSolution();

            double mistake = (results - optCost)/(double)optCost;
            mistake *= 100;
            mistakes[i] = mistake;
        }


    std::fstream file;
    std::string name = "test_results/test_tt_3_ch130.csv";
    file.open(name, std::ios::out);
    for(int i=0; i<100; i++)
        file << mistakes[i] << ";";
    file.close();


    return 0;
}