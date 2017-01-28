//
// Created by piotrek on 01.12.16.
//

#include "TownsTSP.h"

//Private
double TownsTSP::computeDistance(coords A, coords B) {
    double xd = B.x - A.x;
    double yd = B.y - A.y;

    double dist = sqrt(xd*xd + yd*yd);

    return dist;
}
void TownsTSP::permuteRoute(int* route) {
    int a, b;

    a = std::rand()%map_dim;
    do
        b = std::rand()%map_dim;
    while(a == b);


    int buf = route[a];
    route[a] = route[b];
    route[b] = buf;
}
void TownsTSP::permuteRoute(int* route, TabuMatrix& tabu) {
    int a, b;

    TabuMatrix localTabu = *new TabuMatrix(map_dim,1);

    a = std::rand()%map_dim;
    do
        b = std::rand()%map_dim;
    while(a == b || tabu.isTabu(route[a],route[b]) || localTabu.isTabu(route[a], route[b]));

    localTabu.addMove(route[a], route[b]);
    localTabu.addMove(route[b], route[a]);
    tabu.addMove(route[a], route[b]);
    tabu.addMove(route[b], route[a]);

    int buf = route[a];
    route[a] = route[b];
    route[b] = buf;


}
void TownsTSP::swapTowns(int* route, int a, int b) {
    int buf = route[a];
    route[a] = route[b];
    route[b] = buf;
}

//Public
TownsTSP::TownsTSP() {
    problem_name = "";
    map_dim = 0;
    coords_list = nullptr;
    towns_map = nullptr;
    solution = nullptr;
}
TownsTSP::~TownsTSP() {
    delete[] coords_list;
    delete[] solution;
    delete[] towns_map;
}

bool TownsTSP::loadMap(std::string file_name) {
    if (file_name[file_name.length() - 1] != *"p" ||
        file_name[file_name.length() - 2] != *"s" ||
        file_name[file_name.length() - 3] != *"t" ||
        file_name[file_name.length() - 4] != *".")
    {
        /***Złe rozszerzenie pliku***/
        std::cout<<"Złe rozszerzenie pliku! Można wzytać pliki .tsp lub .atsp. !\n";
        return false;
    }

    /* Tworzê strumieñ do odczytu danych z pliku.
    Otwieram go w trybie "std::ios::in", czyli do odczytu.*/
    std::fstream input;
    input.open(file_name.c_str(), std::ios::in);

    /* Sprawdzam, czy powiod³o siê otwarcie pliku.*/
    // if (!input.good())
    // 	throw FileNotFoundException("\nNie ma takiego pliku!");

    std::string input_line;

    getline(input, input_line);

    std::istringstream* iss;
    iss = new std::istringstream(input_line);



    /* Odczytanie nazwy instancji problemu*/
    try
    {
        *iss >> this->problem_name;
        *iss >> this->problem_name;
    }
    catch (std::exception ex)
    {
        std::cerr << "Blad podczas wczytywania danych z pliku:\n" << ex.what();
        return false;
    }
    delete iss;

    std::string dim_info;

    /*Przeskoczenie dwóch niepotrzebnych linii w pliku wejściowym*/
    getline(input, input_line);
    getline(input, input_line);
    getline(input, input_line);

    iss = new std::istringstream(input_line);

    /*Odczytanie liczby miast*/
    try
    {
        *iss >> dim_info;
        *iss >>this->map_dim;
    }
    catch (std::exception ex)
    {
        std::cerr << "Blad podczas wczytywania danych z pliku:\n" << ex.what();
        return false;
    }
    delete iss;


    /*Przeskoczenie dwóch niepotrzebnych linii w pliku wejściowym*/
    getline(input, input_line);
    getline(input, input_line);

    /*Sprawdzenie, czy lista współrzędnych miast jest pusta*/
    if(this->coords_list != nullptr)
        delete[] coords_list;

    /*Tworzenie listy współrzędnych miast*/
    this->coords_list = new coords[map_dim];

    for(int i = 0; i < map_dim; i++)
    {
        getline(input,input_line);
        iss = new std::istringstream(input_line);
        try
        {
            *iss >> this->coords_list[i].x;
            *iss >> this->coords_list[i].x;
            *iss >> this->coords_list[i].y;
        }
        catch (std::exception ex)
        {
            std::cerr << "Blad podczas wczytywania danych z pliku:\n" << ex.what();
            return false;
        }
        delete iss;
    }

    towns_map = new double*[map_dim];

    /*Tworzenie macierzy z kosztami krawędzi*/
    for(int i=0; i<map_dim;i++)
    {
        towns_map[i] = new double[map_dim];
        for(int j=0; j<map_dim; j++)
        {
            if(i == j)
                towns_map[i][j] = -1;
            else
                towns_map[i][j] = computeDistance(coords_list[i], coords_list[j]);


        }
    }

    srand(time(NULL));

    if(solution != nullptr)
        delete[] solution;

    solution = new int[map_dim];
    for(int i = 0; i < map_dim; i++)
        solution[i] = i;

    for(int i = 0; i < 100; i++)
        permuteRoute(solution);

    return true;
}
int TownsTSP::getOptCost() {
    /* Tworzę strumieñ do odczytu danych z pliku.
        Otwieram go w trybie "std::ios::in", czyli do odczytu.*/
    std::fstream input;
    input.open(this->opt_solutions_file, std::ios::in);

    /* Sprawdzam, czy powiod³o siê otwarcie pliku.*/
    if (!input.good())
    {
        std::cout << "Blad! Nie ma takiego pliku!\n";
        return false;
    }

    std::string input_line;
    std::string name;
    std::istringstream* iss;

    int cost;
    do{
        getline(input, input_line);
        iss  = new std::istringstream(input_line);

        *iss >> name;
        if( name == this->problem_name)
        {
            *iss >> cost;
            return cost;
        }

        delete iss;

    }while(name != "EOF");

    return -1;
}
double TownsTSP::routeCost(int *route) {
    int cost = 0;

    for( int i = 0; i+1 < map_dim; i++)
        cost += towns_map[ route[i] ][ route[i+1] ];

    cost += towns_map[ route[0] ][ route[map_dim-1] ];
    return cost;
}
void TownsTSP::resetSolution() {
    for(int j = 0; j < this->map_dim; j++)
        permuteRoute(this->solution);
}
//algorithms
void TownsTSP::performSA() {

    int* s_prim = new int[map_dim];
    int* s = new int[map_dim];

    memcpy(s, solution, map_dim*sizeof(int));

    double T;

    if(map_dim < 100)
        T = 0.5* routeCost(solution);
    else
        T = 1.7* routeCost(solution);

    std::cout << "Problem TSP - instancja " << problem_name << std::endl << "Rozmiar instancji: " << map_dim << std::endl;
    std::cout << "Koszt rozwiazania poczatkowego: " << routeCost(solution) << std::endl <<"Temp poczatkowa: " << T << std::endl;
    std::cout << "Wspolczynnik zmiany temperatury:  " << t_rate << std::endl << "Liczba obrotow petli wewnetrznej: " << L << std::endl;
    std::cout << std::endl << "Rozpoczynam wykonywanie algorytmu..." << std::endl;


    do
    {
        for(int i = 0; i < L; i++)
        {
            /*Losujemy rozwiązanie z sąsiedztwa rozwiązania " s "*/
            memcpy(s_prim, s, map_dim*sizeof(int));

            permuteRoute(s_prim);

            /*Wyliczamy koszt rozwiązania " s_prim " oraz " s "*/
            double s_prim_value = routeCost(s_prim);
            double s_value = routeCost(s);
            double best_value = routeCost(solution);

            /*Jeśli s_prim lepsze od ostatniego najlepszego to je zapamiętujemy.*/
            if(s_prim_value < best_value)
                memcpy(solution, s_prim, map_dim*sizeof(int));

            double sigma = s_prim_value - s_value;
            /*Jeśli s_prim lepsze od s to przypisujemy s_prim do s i kontynuujemy pętlę*/
            if(sigma < 0)
                memcpy(s, s_prim, map_dim*sizeof(int));
            else
            {
                /*Losujemy x z przedziału (0,1> i liczymy wartość funkcji exp*/
                double x = (double)(std::rand()/RAND_MAX);
                double exp_fun = exp( (-1)*sigma/T );

                if(x < exp_fun)
                    memcpy(s, s_prim, map_dim*sizeof(int));
                else
                if(s_value < best_value)
                    memcpy(solution, s, map_dim*sizeof(int));
            }
        }

        T = t_rate * T;
    }while(T > 0.001);

    delete[] s_prim;
    delete[] s;
    return;
}
void TownsTSP::deterministicTabu(int tt, int it) {
    //random initial solution
    int* solutionC = new int [map_dim];
    memcpy(solutionC,solution, map_dim*sizeof(int));

    //array containing positions of the cities in the solutionC
    int* positionsC = new int [map_dim];
    for (int i = 0; i < map_dim; ++i)
        positionsC[solutionC[i]] = i;

    //global best solution cost
    double globalLowest = routeCost(solution);
    double solutionC_cost = routeCost(solutionC);
    int iterations = 0;
    int resetsDone = 0;
    bool stopCriteria = false;

    //tabu matrix
    TabuMatrix tabu = *new TabuMatrix(map_dim, tt);

    while(!stopCriteria) {

        double neighbourhoodLowest = DBL_MAX;
        int bestLeft = -1, bestRight=-1;

        ///Find best solution in the neighbourhood of solutionC (neighbourhood is all possible town swaps in solutionC)
        for (int i = 0; i < map_dim - 1 ; ++i) {
            for (int j = i + 1; j < map_dim; ++j) {

                //we check aspiration
                if(tabu.isTabu(i,j)){
                    swapTowns(solutionC,positionsC[i], positionsC[j]);
                    //calculating the cost of neighbour solution
                    double tempCost = routeCost(solutionC);
                    //if move on tabu leads to solution better then ever
                    if(tempCost < globalLowest && tempCost < neighbourhoodLowest){
                        neighbourhoodLowest = tempCost;
                        bestLeft = i;
                        bestRight = j;
                    }
                    //come back to initial solution
                    swapTowns(solutionC, positionsC[i], positionsC[j]);
                    continue;

                }

                swapTowns(solutionC, positionsC[i], positionsC[j]);
                //calculating the cost of neighbour solution
                double tempCost = routeCost(solutionC);


                //if the move leads to the currently best solution in neighbourhood then save it as currently best
                if (tempCost < neighbourhoodLowest) {
                    neighbourhoodLowest = tempCost;
                    bestLeft = i;
                    bestRight = j;

                }

                //come back to initial solution
                swapTowns(solutionC, positionsC[i], positionsC[j]);
            }
        }

        if(solutionC_cost - neighbourhoodLowest < 20)
            iterations++;

        //add the move to tabu
        tabu.addMove(bestLeft, bestRight);
        tabu.addMove(bestRight, bestLeft);

        //update the positions list
        int buf = positionsC[bestLeft];
        positionsC[bestLeft] = positionsC[bestRight];
        positionsC[bestRight] = buf;
        swapTowns(solutionC, positionsC[bestLeft], positionsC[bestRight]);
        solutionC_cost = neighbourhoodLowest;


        //if solutionC is now better then global best solution, then update the global best
        if (solutionC_cost < globalLowest) {
            delete[] solution;
            solution = new int[map_dim];
            memcpy(solution, solutionC, map_dim * sizeof(int));
            globalLowest = solutionC_cost;

        }

        if(iterations == it){
            resetsDone++;
            iterations = 0;
            delete[] solutionC;
            solutionC = new int [map_dim];
            memcpy(solutionC, solution, map_dim*sizeof(int));
            for(int i = 0; i < 10; i++)
                permuteRoute(solutionC,tabu);

            solutionC_cost = routeCost(solutionC);

        }
        if(resetsDone == 5)
            stopCriteria = true;
        tabu.decrementTenure();
    }
}
void TownsTSP::randomNeighbourhoodTabu(int tt, int it, int nSize) {
    //random initial solution
    int* solutionC = new int [map_dim];
    memcpy(solutionC,solution, map_dim*sizeof(int));

    //array containing positions of the cities in the solutionC
    int* positionsC = new int [map_dim];
    for (int i = 0; i < map_dim; ++i)
        positionsC[solutionC[i]] = i;

    //global best solution cost
    double globalLowest = routeCost(solution);
    double solutionC_cost = routeCost(solutionC);

    int iterations = 0;
    int resetsDone = 0;
    bool stopCriteria = false;

    //tabu matrix
    TabuMatrix tabu = *new TabuMatrix(map_dim, tt);

    while(!stopCriteria) {

        double neighbourhoodLowest = DBL_MAX;
        int bestLeft = -1, bestRight=-1;

        ///Find best solution in the neighbourhood of solutionC (neighbourhood is nSize random town swaps)
        for (int i = 0; i < nSize; ++i) {

            int a;
            int b;
            do{
                a = rand()%map_dim;
                b = rand()%map_dim;
            }while(a == b && !tabu.isTabu(a,b) );

            //we check aspiration
            if(tabu.isTabu(solutionC[positionsC[a]], solutionC[positionsC[b]])) {
               swapTowns(solutionC, positionsC[a], positionsC[b]);
                //calculating the cost of neighbour solution
                double tempCost = routeCost(solutionC);
                //if move on tabu leads to solution better then ever
                if (tempCost < globalLowest && tempCost < neighbourhoodLowest) {
                    neighbourhoodLowest = tempCost;
                    bestLeft = a;
                    bestRight = b;
                }
                //come back to initial solution
                swapTowns(solutionC, positionsC[a], positionsC[b]);
                continue;
            }

            swapTowns(solutionC, positionsC[a], positionsC[b]);
            //calcule the cost of neighbour solution
            double tempCost = routeCost(solutionC);


            //if the move leads to the currently best solution in neighbourhood then save it as currently best
            if (tempCost < neighbourhoodLowest) {
                neighbourhoodLowest = tempCost;
                bestLeft = a;
                bestRight = b;

            }

            //come back to initial solution
            swapTowns(solutionC, positionsC[a], positionsC[b]);
        }

        if(solutionC_cost - neighbourhoodLowest < 20)
            iterations++;

        //Add the move to tabu
        tabu.addMove(bestLeft, bestRight);
        tabu.addMove(bestRight, bestLeft);
        swapTowns(solutionC, positionsC[bestLeft], positionsC[bestRight]);

        //Update the positions list
        int buf = positionsC[bestLeft];
        positionsC[bestLeft] = positionsC[bestRight];
        positionsC[bestRight] = buf;

        solutionC_cost = neighbourhoodLowest;


        //if solutionC is now better then global best solution, then update the global best
        if (solutionC_cost < globalLowest) {
            delete[] solution;
            solution = new int[map_dim];
            memcpy(solution, solutionC, map_dim * sizeof(int));
            globalLowest = solutionC_cost;
        }

        if(iterations == it){
            resetsDone++;
            iterations = 0;
            delete[] solutionC;
            solutionC = new int [map_dim];
            memcpy(solutionC, solution, map_dim*sizeof(int));
            for(int i = 0; i < 10; i++)
                permuteRoute(solutionC,tabu);

            solutionC_cost = routeCost(solutionC);

        }
        if(resetsDone == 5)
            stopCriteria = true;
        tabu.decrementTenure();
    }
}

void TownsTSP::genetic(int generations, int populationSize, double elitarismFactor, int matingPopulationSize, SelectionStrategy selectionStartegy, CrossoverStrategy crossoverStrategy) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    ///INICJALIZACJA
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ///Liczba genów w chromosomie
    int genesNum = this->map_dim;
    ///Bazowe rozwiązanie - potrzebne do dekodowania chromosomów
    int* root = new int[genesNum];
    for(int i = 0; i< genesNum; i++)
        root[i] = i;
    ///Utworzenie populacji bierzącego pokolenia
    Specimen** currentPopulation = new Specimen*[populationSize];
    ///Tablica przechowujaca indeksy osobników wybranych do krzyżowania
    Specimen** matingPopulation = new Specimen*[matingPopulationSize];
    int matingCouplesNum = populationSize;

    ///Wygenerowanie losowej populacji startowej i zsumowanie przystosowań
    ///Zmienna na sumę wartości przystosowań dla całego pokolenia
    ///Przystosowania mają wartości od 1 do rozmiaru populacji (1 = najgorsze przystosowanie)
    int fitnessSum = 0;


    ///Wygenerowanie losowej populacji początkowej
    generateRandomPopulation(currentPopulation, populationSize, genesNum);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    ///ALGORYTM
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    ///Pętla przechodząca po wszystkich pokoleniach
    for (int gen = 0; gen < generations; ++gen) {

        fitnessSum = 0;
        ///Ocena osobników populacji
        rankAndSortPopulation(currentPopulation,populationSize,root,fitnessSum);

//        for(int i = 0; i<10;i++){
//            std::cout<<"dł drogi: "<<currentPopulation[i]->getCost() << " ";
//            int* chromosome = currentPopulation[i]->getChromosome();
//            std::cout<<"[ ";
//            for (int j = 0; j < genesNum ; ++j) {
//                std::cout<<chromosome[j]<<" ";
//            }
//            std::cout<<"]"<<std::endl;
//        }
//
//        std::cout<<"///////////////////////////////////"<<std::endl;

        ///Porównanie z najlepszym dotychczas znalezionym
        if(currentPopulation[0]->getCost() < routeCost(this->solution)) {
            currentPopulation[0]->decodeChromosome(root, this->solution);
            //std::cout<<"poprawa w populacji nr "<<gen<<std::endl;
        }

        ///Selekcja osobników do populacji macierzystej
        for (int i = 0; i < matingPopulationSize; ++i) {
            switch(selectionStartegy){
                case SelectionStrategy::ROULETTE:{
                    matingPopulation[i] = roulette(currentPopulation, populationSize, fitnessSum);
                    break;
                }
                case SelectionStrategy::TOURNAMENT:{
                    matingPopulation[i] = tournament(currentPopulation, populationSize, 10);
                    break;
                }
            }
        }

        ///Krzyżowanie osobników z populacji macierzystej
        Specimen** offspringPopulation = new Specimen*[2*matingCouplesNum];
        int offspringBorn = 0;

        for (int j = 0; j < matingCouplesNum; ++j) {

            int crossoverProbability = 95;
            int throwDice = 1 + rand()%100;
            if(throwDice <= crossoverProbability){
                ///Losowanie pary rodziców z populacji macierzystej
                int parentAIndex = rand()%matingPopulationSize;
                int parentBIndex = rand()%matingPopulationSize;
                while(parentAIndex == parentBIndex)
                    parentBIndex = rand()%matingPopulationSize;
                ///Tworzenie potomków za pomocą jednej z tzech metod krzyżowania
                Specimen* offspringAB = new Specimen(genesNum);
                Specimen* offspringBA = new Specimen(genesNum);

                switch (crossoverStrategy){
                    case CrossoverStrategy::RANDOM:{
                        randomCrossover(matingPopulation[parentAIndex], matingPopulation[parentBIndex], offspringAB, offspringBA, genesNum);
                        break;
                    }
                    case CrossoverStrategy::ONEPOINT:{
                        onePointCrossover(matingPopulation[parentAIndex], matingPopulation[parentBIndex], offspringAB, offspringBA, genesNum);
                        break;
                    }
                    case CrossoverStrategy::TWOPOINT:{
                        twoPointCrossover(matingPopulation[parentAIndex], matingPopulation[parentBIndex], offspringAB, offspringBA, genesNum);
                        break;
                    }
                }

                mutate(offspringAB, root,genesNum,10);
                mutate(offspringBA,root,genesNum,10);

                ///Dodanie stworzonych potomków do populacji potomków
                offspringPopulation[offspringBorn] = offspringAB;
                offspringPopulation[offspringBorn+1] = offspringBA;
                offspringBorn += 2;
            }
        }
        ///Utworzenie populacji zbiorczej
        Specimen** bigPopulation = new Specimen*[populationSize+offspringBorn];
        memcpy(bigPopulation,currentPopulation,populationSize*sizeof(Specimen*));
        memcpy(bigPopulation+populationSize, offspringPopulation, offspringBorn*sizeof(Specimen*));
        fitnessSum = 0;
        rankAndSortPopulation(bigPopulation,populationSize+offspringBorn,root,fitnessSum);
        ///Skopiowanie najlepszych osobników do nowej populacji
        memcpy(currentPopulation,bigPopulation, populationSize*sizeof(Specimen*));
        ///Zwolnienie pamięcie po osobnikach odrzuconych
        for(int i=populationSize; i < populationSize+offspringBorn; i++)
            delete bigPopulation[i];

    }

//    for(int i = 0; i<populationSize;i++){
//        std::cout<<"dł drogi: "<<currentPopulation[i]->getCost() << " ";
//        int* chromosome = currentPopulation[i]->getChromosome();
//        std::cout<<"[ ";
//        for (int j = 0; j < genesNum ; ++j) {
//            std::cout<<chromosome[j]<<" ";
//        }
//        std::cout<<"]"<<std::endl;
//    }
    delete[] root;
}

void TownsTSP::generateRandomPopulation(Specimen **population, int populationSize, int genesNum) {
    for (int i = 0; i < populationSize; i++) {
        population[i] = new Specimen(genesNum);
        int chromosome[genesNum];

        for (int j = 0; j < genesNum; j++) {
            int topBorder = genesNum-j;
            int gene = rand()%topBorder;
            chromosome[j] = gene;
        }
        population[i]->setChromosome(chromosome);
    }
}

double TownsTSP::getSpecimenRouteCost(Specimen *specimen, int *root) {
    int* route = new int [specimen->getGenesNum()];
    specimen->decodeChromosome(root, route);
    double cost = routeCost(route);
    delete[] route;
    return cost;
}

Specimen *TownsTSP::tournament(Specimen **population, int populationSize, int tournamentMembers) {
    Specimen* best = nullptr;
    int lowest = populationSize;
    for (int i = 0; i < tournamentMembers; ++i) {
        int index = rand()%populationSize;
        if(index < lowest)
            lowest = index;
    }

    best = population[lowest];
    return best;
}

void TownsTSP::onePointCrossover(Specimen *parent1, Specimen *parent2, Specimen *child1, Specimen *child2, int genesNum) {
    int crossoverPoint = 1 + rand()%(genesNum-1);

    int* chromosome = new int[genesNum];
    //pierwsze dziecko
    memcpy(chromosome, parent1->getChromosome(), crossoverPoint*sizeof(int));
    memcpy(chromosome+crossoverPoint, parent2->getChromosome()+crossoverPoint, (genesNum-crossoverPoint)*sizeof(int));
    child1->setChromosome(chromosome);

    //drugie dziecko
    memcpy(chromosome, parent2->getChromosome(), crossoverPoint*sizeof(int));
    memcpy(chromosome+crossoverPoint, parent1->getChromosome()+crossoverPoint, (genesNum-crossoverPoint)*sizeof(int));
    child2->setChromosome(chromosome);
}

Specimen *TownsTSP::roulette(Specimen **population,int populationSize, int fitnessSum) {
    int throwDice = rand()%fitnessSum; // losujemy liczbę od 0 - sumy przystosowań
    int sum = 0;
    for (int i = 0; i < populationSize; ++i) {
        sum += (populationSize-i);
        if(sum > throwDice)
            return population[i];
    }
}

void TownsTSP::twoPointCrossover(Specimen *parent1, Specimen *parent2, Specimen *child1, Specimen *child2, int genesNum) {
    int firstCrossoverPoint = 1 + rand()%(genesNum-2);
    int secondCrossoverPoint = 1+ rand()%(genesNum-firstCrossoverPoint-1);


    int* chromosome = new int[genesNum];
    //pierwsze dziecko
    memcpy(chromosome, parent1->getChromosome(), firstCrossoverPoint*sizeof(int));
    memcpy(chromosome+firstCrossoverPoint, parent2->getChromosome()+firstCrossoverPoint, secondCrossoverPoint*sizeof(int));
    memcpy(chromosome+firstCrossoverPoint+secondCrossoverPoint, parent1->getChromosome()+firstCrossoverPoint+secondCrossoverPoint, (genesNum-firstCrossoverPoint-secondCrossoverPoint)*sizeof(int));
    child1->setChromosome(chromosome);

    //drugie dziecko
    memcpy(chromosome, parent2->getChromosome(), firstCrossoverPoint*sizeof(int));
    memcpy(chromosome+firstCrossoverPoint, parent1->getChromosome()+firstCrossoverPoint, secondCrossoverPoint*sizeof(int));
    memcpy(chromosome+firstCrossoverPoint+secondCrossoverPoint, parent2->getChromosome()+firstCrossoverPoint+secondCrossoverPoint, (genesNum-firstCrossoverPoint-secondCrossoverPoint)*sizeof(int));
    child2->setChromosome(chromosome);
}

void TownsTSP::mutate(Specimen *specimen,int* root, int genesNum, int mutationProbability) {

    int throwDice = rand()%1000;
    if(throwDice < mutationProbability) {

        int route[genesNum];
        specimen->decodeChromosome(root, route);

        //////////////////////////////////////////////

//        std::cout<<"Chromosom przed mutacją:\n[";
//        for(int i = 0; i<genesNum;i++)
//            std::cout<<specimen->getChromosome()[i] << " ";
//        std::cout<<"]"<<std::endl;

        ///////////////////////////////////////////

        //////////////////////////////////////////////

//        std::cout<<"Sciezka przed mutacją:\n[";
//        for(int i = 0; i<genesNum;i++)
//            std::cout<<route[i] << " ";
//        std::cout<<"]"<<std::endl;

        ///////////////////////////////////////////

        ///Wykonanie mutacji invert na rozkodowanym chromosomie
        int a = rand()%genesNum;
        int b = a;
        while (b == a)
            b = rand()%genesNum;
        if(a > b){
            int buf = a;
            a = b;
            b = buf;
        }

        int invertSize = b - a + 1;
        int invBuf[invertSize];
        ///Odwrócenie kolejności mutowanego fragmentu
        for(int i = 0; i < invertSize; i++)
            invBuf[i] = route[b - i];
        ///Zapisanie zmutowanego fragmentu z powrotem na ścieżkę
        for(int i = 0; i < invertSize; i++)
            route[a+i] = invBuf[i];

        ///Zakodowanie nowej ścieżki
        bool alreadyIn[genesNum];
        for(int i = 0; i< genesNum;i++)
            alreadyIn[i] = false;
        for (int i = 0; i < genesNum; ++i) {
            int index = 0;
            for(int k = 0; k < route[i]; k++)
                if(!alreadyIn[k])
                    index++;
            specimen->getChromosome()[i]=index;
            alreadyIn[route[i]] = true;
        }

        //////////////////////////////////////////////

//        std::cout<<"Chromosom po mutacjii:\n[";
//        for(int i = 0; i<genesNum;i++)
//            std::cout<<specimen->getChromosome()[i] << " ";
//        std::cout<<"]"<<std::endl;

        ///////////////////////////////////////////
        int* decoded = new int [genesNum];
        specimen->decodeChromosome(root, decoded);


        //////////////////////////////////////////////
//
//        std::cout<<"Sciezka po mutacji:\n[";
//        for(int i = 0; i<genesNum;i++)
//            std::cout<<decoded[i] << " ";
//        std::cout<<"]"<<std::endl;

        ///////////////////////////////////////////
    }
}

void TownsTSP::randomCrossover(Specimen *parent1, Specimen *parent2, Specimen *child1, Specimen *child2, int genesNum) {
    for (int i = 0; i < genesNum; ++i) {
        if(rand()%2) {
            child1->getChromosome()[i] = parent1->getChromosome()[i];
            child2->getChromosome()[i] = parent2->getChromosome()[i];
        }else{
            child1->getChromosome()[i] = parent2->getChromosome()[i];
            child2->getChromosome()[i] = parent1->getChromosome()[i];
        }

    }
}

void TownsTSP::rankAndSortPopulation(Specimen** population, int populationSize, int* root, int& fitnessSum){
    for (int i = 0; i < populationSize; ++i) {
        fitnessSum += i+1;
        ///Obliczenie kosztu drogi reprezentowanej przez danego osobnika i zapisanie tej wartości
        double specimenRouteCost = getSpecimenRouteCost(population[i], root);
        population[i]->setRouteCost(specimenRouteCost);

        ///Sortowanie osobników od najlepszego do najgorszego według kosztów dróg.
        if (i > 0) {
            int head = i;
            while (head >= 1 && population[head]->getCost() < population[head - 1]->getCost()) {
                ///Zamiana osobników miejscami, tak aby lepszy był na niższym indeksie
                Specimen* buf = population[head];
                population[head] = population[head - 1];
                population[head - 1] = buf;
                head--;
            }

        }
    }
}

























































