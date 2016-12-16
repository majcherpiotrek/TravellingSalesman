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

        double results = 0;
        double mistake = 0;
        int optCost = townsT.getOptCost();

        double mistakes[11];
        double times[11];

        int it[] = {100,150,200,250,300,350,400,450,500,550,600};
        double TT[] = {0.25,0.5,0.75,1,1.25,1.5,1.75,2,2.5,3.0,3.5};
        clock_t START, STOP;
        double tT;
        for(int j=0; j<11; j++) {

            std::cout<<"iterations: " << it[j] <<std::endl;
            for (int i = 0; i < 30; i++) {

                std::cout<< i << std::endl;
                START = clock();
                townsT.deterministicTabu((int)(TT[j]*townsT.getSize()), 200);
                STOP = clock();

                tT += (double) (STOP - START) / CLOCKS_PER_SEC * 1000;


                results = townsT.routeCost(townsT.solution);
                mistake += (results - optCost) / (double) optCost;
                townsT.resetSolution();

            }

            mistake *= 100;
            mistake = mistake/30;

            tT = tT/30;

            mistakes[j] = mistake;
            times[j] = tT;

            mistake = 0;
            tT = 0;

        }


    std::fstream file;
    std::string name = "test_results/berlin52_iterations_test.csv";
    file.open(name, std::ios::out);
    for(int i=0; i<11; i++)
        file << TT[i] << ";";
    file << std::endl;
    for(int i=0; i<11; i++)
        file << mistakes[i] << ";";
    file << std::endl;
    for(int i=0; i<11; i++)
        file << times[i] << ";";
    file.close();


    return 0;
}