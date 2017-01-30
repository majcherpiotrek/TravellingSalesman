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
        cost += towns_map[route[i]][route[i + 1]];

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
void TownsTSP::newGenetic(int generations, int popSize, int matingPopSize, int mutationProb) {

    int genesNum = this->map_dim;
    int crossoverProb = 95;
    ///Tablica przechowująca pokolenie
    int* population[popSize];
    ///Tablica przechowująca koszty dróg reprezentowanych przez poszczególnych osobników
    double routeCostsTable[popSize];
    int fitnessSum = 0;
    ///Wygenerowanie populacji początkowej
    for (int i = 0; i < popSize ; ++i) {
        fitnessSum += i+1;
        population[i] = new int[genesNum];
        for (int j = 0; j < genesNum; ++j)
            population[i][j] = j;
        for(int k = 0; k < 10; k++)
            invertMutate(population[i],genesNum);
        routeCostsTable[i] = routeCost(population[i]);
    }

    int* matingPopulation[matingPopSize];
    int matingCouples = popSize;

//    std::cout<<"[ ";
//    for (int i = 0; i < popSize; ++i) {
//        for(int k = 0; k < genesNum; k++)
//            std::cout<<population[i][k] << " ";
//        std::cout<<"]"<<std::endl;
//    }
    for (int gen = 0; gen < generations; ++gen) {

        ///Posortowanie populacji
        quicksort(routeCostsTable,population,0,popSize-1);

        ///Porównanie z najlepszym dotychczas znalezionym rozwiązaniem
        if(routeCostsTable[0] < routeCost(this->solution)){
            memcpy(this->solution,population[0],genesNum*sizeof(int));
        }

        ///Selekcja osobników do populacji macierzystej za pomocą ruletki
        for (int i = 0; i < matingPopSize; ++i)
            matingPopulation[i] = population[rouletteSelection(population,routeCostsTable,popSize,fitnessSum)];

        int** offspringPopulation = new int*[2*matingCouples];
        double* offspringRouteCosts = new double[2*matingCouples];
        int offspringBorn = 0;

        ///Rozmnażanie osobników z populacji macierzystej
        for (int j = 0; j < matingCouples; ++j) {
            int p = 1 + rand()%100;
            ///Krzyżyjemy z pewnym prawdopodobieństwem
            if( p < crossoverProb){
                ///Wylosowanie pary rodziców
                int parentA = rand()%matingPopSize;
                int parentB = rand()%matingPopSize;
                while (parentA == parentB)
                    parentB = rand()%matingPopSize;

                ///Tworzenie dzieci
                int* childAB = new int[genesNum];
                int* childBA = new int[genesNum];

                ///Krzyżowanie metodą OX
                OXcrossover(matingPopulation[parentA],matingPopulation[parentB], childAB, childBA, genesNum);

                ///Mutowanie dzieci z pewnym prawdopodobieństwem
                int mut = rand()%1000;
                if(mut < mutationProb)
                    invertMutate(childAB,genesNum);

                mut = rand()%1000;
                if(mut < mutationProb)
                    invertMutate(childBA,genesNum);



                offspringPopulation[offspringBorn] = new int[genesNum];
                offspringPopulation[offspringBorn+1] = new int[genesNum];
                memcpy(offspringPopulation[offspringBorn],childAB,genesNum*sizeof(int));
                memcpy(offspringPopulation[offspringBorn+1],childBA,genesNum*sizeof(int));
                offspringRouteCosts[offspringBorn] = routeCost(childAB);
                offspringRouteCosts[offspringBorn+1] = routeCost(childBA);
                offspringBorn += 2;

                delete[] childAB;
                delete[] childBA;
            }
        }

        int* bigPopulation[popSize+offspringBorn];
        memcpy(bigPopulation,population,popSize*sizeof(int*));
        memcpy(bigPopulation+popSize,offspringPopulation,offspringBorn*sizeof(int*));
        double* bigPopCostTable = new double[popSize+offspringBorn];
        memcpy(bigPopCostTable,routeCostsTable,popSize*sizeof(double));
        memcpy(bigPopCostTable+popSize,offspringRouteCosts,offspringBorn*sizeof(double));

        quicksort(bigPopCostTable, bigPopulation, 0, popSize+offspringBorn-1);

        memcpy(population,bigPopulation,popSize*sizeof(int*));
        delete[] offspringRouteCosts;
        for(int i = popSize; i<popSize+offspringBorn; i++){
            delete[] bigPopulation[i];
        }

        memcpy(routeCostsTable,bigPopCostTable,popSize*sizeof(double));
        delete[] bigPopCostTable;
       //std::cout<<"generations: "<<gen<<std::endl;
    }
}
void TownsTSP::invertMutate(int *chromosome, int genesNum) {
    int a = rand()%genesNum;
    int b = a;
    while (a == b)
        b = rand()%genesNum;
    if(a > b){
        int buf = a;
        a = b;
        b = buf;
    }

    int invertRange = b - a + 1;
    int invertBuf[invertRange];

    for (int i = 0; i < invertRange; ++i)
        invertBuf[i] = chromosome[b-i];
    for (int i = 0; i < invertRange; ++i)
        chromosome[a+i] = invertBuf[i];
}
int TownsTSP::rouletteSelection(int **population, double *costsTable, int popSize, int fitnessSum) {
    int throwDice = rand()%fitnessSum; // losujemy liczbę od 0 - sumy przystosowań
    int sum = 0;
    for (int i = 0; i < popSize; ++i) {
        sum += (popSize-i);
        if(sum > throwDice)
            return i;


    }
}
void TownsTSP::OXcrossover(int *parentA, int *parentB, int *childAB, int *childBA, int genesNum) {
    int a = rand()%genesNum;
    int b = rand()%genesNum;
    while (a==b)
        b = rand()%genesNum;
    if(a > b){
        int buf = a;
        a = b;
        b = buf;
    }

    int crossSectionSize = b - a + 1;
    int crossA[crossSectionSize];
    int crossB[crossSectionSize];
    for(int i = 0; i < crossSectionSize; i++){
        childAB[a+i] = parentB[a+i];
        crossB[i] = parentB[a+i];
        childBA[a+i] = parentA[a+i];
        crossA[i] = parentA[a+i];
    }

    int genesLeft = genesNum - crossSectionSize;
    int k = (b == genesNum-1) ? 0 : b+1;
    int toPut = k;
    for(int i = 0; i < genesLeft; i++){
        bool conflict = true;
        while(conflict){
            ///Porównujemy z wszystkim elementami w sekcji dopasowania
            for (int j = 0; j < crossSectionSize; ++j) {
                ///Jeśłi występuje konflikt
                if(parentA[toPut] == crossB[j]){
                    conflict = true;
                    break;
                }
                else
                    conflict = false;
            }
            ///Jeśli był konflikt to sprawdzamy kolejny element
            if(conflict)
                if(toPut==genesNum-1)
                    toPut=0;
                else
                    toPut++;
        }
        childAB[k] = parentA[toPut];
        if(k==genesNum-1)
            k=0;
        else
            k++;

        if(toPut == genesNum-1)
            toPut=0;
        else
            toPut++;
    }

    k = (b == genesNum-1) ? 0 : b+1;
    toPut = k;
    for(int i = 0; i < genesLeft; i++){
        bool conflict = true;
        while(conflict){
            ///Porównujemy z wszystkim elementami w sekcji dopasowania
            for (int j = 0; j < crossSectionSize; ++j) {
                ///Jeśłi występuje konflikt
                if(parentB[toPut] == crossA[j]){
                    conflict = true;
                    break;
                }
                else
                    conflict = false;
            }
            ///Jeśli był konflikt to sprawdzamy kolejny element
            if(conflict)
                if(toPut==genesNum-1)
                    toPut=0;
                else
                    toPut++;
        }
        childBA[k] = parentB[toPut];
        if(k==genesNum-1)
            k=0;
        else
            k++;

        if(toPut == genesNum-1)
            toPut=0;
        else
            toPut++;
    }
}
void TownsTSP::quicksort(double* costsArray, int** population, int left, int right){
    double v = costsArray[(left+right)/2];
    int i = left;
    int j = right;
    double costBuf;
    int* buf;
    do{
        while(costsArray[i]<v) i++;
        while (costsArray[j]>v) j--;
        if(i<=j){
            costBuf = costsArray[i];
            buf = population[i];

            costsArray[i]=costsArray[j];
            population[i]=population[j];

            costsArray[j]=costBuf;
            population[j]=buf;

            i++;j--;
        }
    }while (i <=j);

    if(j>left)
        quicksort(costsArray,population,left,j);
    if(i<right)
        quicksort(costsArray,population,i,right);
}

























































