//
// Created by piotrek on 01.12.16.
//

#include "TownsATSP.h"

TownsATSP::TownsATSP()
{
    problem_name = "";
    map_dim = 0;
    towns_map = nullptr;
    solution = nullptr;
}

TownsATSP::~TownsATSP()
{
    if(towns_map != nullptr)
    {
        for(int i = 0; i < map_dim; i++)
            if(towns_map[i] != nullptr)
                delete[] towns_map[i];
        delete[] towns_map;
    }

    if(solution != nullptr)
        delete[] solution;
}

void TownsATSP::permuteRoute(int* route)
{
    int a, b;

    a = std::rand()%map_dim;

    do
        b = std::rand()%map_dim;
    while(a == b );


    int buf = route[a];
    route[a] = route[b];
    route[b] = buf;
}

bool TownsATSP::loadMap(std::string file_name)
{
    if (file_name[file_name.length() - 1] != *"p" ||
        file_name[file_name.length() - 2] != *"s" ||
        file_name[file_name.length() - 3] != *"t" ||
        file_name[file_name.length() - 4] != *"a" ||
        file_name[file_name.length() - 5] != *"."
            )
    {
        /***Złe rozszerzenie pliku***/
        std::cout<<"Złe rozszerzenie pliku! Można wczytać pliki .tsp lub .atsp. !\n";
        return false;
    }

    /* Tworzę strumieñ do odczytu danych z pliku.
    Otwieram go w trybie "std::ios::in", czyli do odczytu.*/
    std::fstream input;
    input.open(file_name.c_str(), std::ios::in);

    /* Sprawdzam, czy powiod³o siê otwarcie pliku.*/
    if (!input.good())
    {
        std::cout << "Blad! Nie ma takiego pliku!\n";
        return false;
    }

    std::string input_line;
    std::string str ="";
    std::istringstream* iss;
    int counter = 0;

    /*Wczytywanie opisu instancji problemu*/
    do
    {
        getline(input, input_line);


        iss = new std::istringstream(input_line);
        /* Szukanie miejsca, gdzie zaczynają się dane*/
        try
        {
            *iss >> str;

            if( str == "NAME:")
            {
                *iss >> this->problem_name;
            }

            if( str == "DIMENSION:")
            {
                *iss >> this->map_dim;

            }

        }
        catch (std::exception ex)
        {
            std::cerr << "Blad podczas wczytywania danych z pliku:\n" << ex.what();
            return false;
        }
        delete iss;
        counter++;

        if(counter > 10)
        {
            std::cout << "Blad podczas wczytywania danych z pliku:\n";
            return false;
        }

    }while(str != "EDGE_WEIGHT_SECTION");

    /*Wczytywanie macierzy wag krawędzi*/
    int buf;

    this->towns_map = new int*[map_dim];
    for(int i = 0; i < map_dim; i++)
        this->towns_map[i] = new int[map_dim];

    getline(input, input_line);
    iss = new std::istringstream(input_line);

    //Liczniki wierszy i kolumn macierzy
    int j = 0;
    int k = 0;

    /*Pętla sczytująca dane do mapy*/
    while(j < map_dim)
    {
        while(*iss >> buf)
        {
            towns_map[j][k] = buf;
            k++;

            if(k == map_dim)
            {
                k = 0;
                j++;
                if(j == map_dim)
                    break;
            }
        }
        if(j < map_dim)
        {
            delete iss;
            getline(input, input_line);
            iss = new std::istringstream(input_line);
        }
    }

    /*Utworzenie losowego rozwiązania*/
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

int TownsATSP::getOptCost()
{
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

int TownsATSP::routeCost(int *route)
{
    int cost = 0;

    for( int i = 0; i+1 < map_dim; i++)
        cost += towns_map[ route[i] ][ route[i+1] ];

    return cost;
}

void TownsATSP::performSA()
{
    int* s_prim = new int[map_dim];
    int* s = new int[map_dim];

    memcpy(s, solution, map_dim*sizeof(int));

    double T = routeCost(solution);


    std::cout << "Problem ATSP - instancja " << problem_name << std::endl << "Rozmiar instancji: " << map_dim << std::endl;
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
            int s_prim_value = routeCost(s_prim);
            int s_value = routeCost(s);
            int best_value = routeCost(solution);

            /*Jeśli s_prim lepsze od ostatniego najlepszego to je zapamiętujemy.*/
            if(s_prim_value < best_value)
                memcpy(solution, s_prim, map_dim*sizeof(int));

            int sigma = s_prim_value - s_value;
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

void TownsATSP::resetSolution() {
    for(int i=0; i<100; i++)
        permuteRoute(this->solution);
}

void TownsATSP::deterministicTabu(int tt) {

    /*Local search*/

    /*random initial solution*/
    int* solutionC = new int [map_dim];
    //for (int k = 0; k < map_dim ; ++k)
    //solutionC[k] = k;
    memcpy(solutionC,solution, map_dim*sizeof(int));

    /*array containing positions of the cities in the solutionC*/
    int* positionsC = new int [map_dim];
    for (int i = 0; i < map_dim; ++i)
        positionsC[solutionC[i]] = i;

    /*global best solution cost*/
    double globalLowest = routeCost(solution);
    double solutionC_cost = routeCost(solutionC);
    int iterations = 0;
    int resetsDone = 0;
    bool stopCriteria = false;

    /*tabu matrix*/
    int tabuTenure = tt;
    TabuMatrix tabu = *new TabuMatrix(map_dim, tabuTenure);

    while(!stopCriteria) {
        /*find best neighbour of solutionC*/
        double neighbourhoodLowest = DBL_MAX;
        int bestLeft = -1, bestRight=-1;
        for (int i = 0; i < map_dim - 1; ++i) {
            for (int j = i + 1; j < map_dim; ++j) {

                /*we check aspiration*/
                if(tabu.isTabu(solutionC[i], solutionC[j])){
                    swapTowns(solutionC, i, j);
                    /*calculating the cost of neighbour solution*/
                    double tempCost = routeCost(solutionC);
                    /*if move on tabu leads to solution better then ever*/
                    if(tempCost < globalLowest && tempCost < neighbourhoodLowest){
                        neighbourhoodLowest = tempCost;
                        bestLeft = i;
                        bestRight = j;
                    }
                    /*come back to initial solution*/
                    swapTowns(solutionC, i, j);
                    continue;
                }

                swapTowns(solutionC, i, j);
                /*calculating the cost of neighbour solution*/
                double tempCost = routeCost(solutionC);


                /*if the move leads to the currently best solution in neighbourhood then save it as currently best*/
                if (tempCost < neighbourhoodLowest) {
                    neighbourhoodLowest = tempCost;
                    bestLeft = i;
                    bestRight = j;
                }

                /*come back to initial solution*/
                swapTowns(solutionC, i, j);
            }
        }

        if(solutionC_cost - neighbourhoodLowest < 10)
            iterations++;
        /*add the move to tabu*/
        tabu.addMove(solutionC[bestLeft], solutionC[bestRight]);
        /*update the positions list*/
        positionsC[solutionC[bestLeft]] = bestRight;
        positionsC[solutionC[bestRight]] = bestLeft;
        swapTowns(solutionC, bestLeft, bestRight);
        solutionC_cost = neighbourhoodLowest;


        /*if solutionC is now better then global best solution, then update the global best*/
        if (solutionC_cost < globalLowest) {
            delete[] solution;
            solution = new int[map_dim];
            memcpy(solution, solutionC, map_dim * sizeof(int));
            globalLowest = solutionC_cost;
            if(resetsDone > 0)
                std::cout <<"poprawa po resecie ATSP nr" << resetsDone << std::endl;
        }


        if(iterations == 10){
            resetsDone++;
            iterations = 0;

            for(int i = 0; i < 2*map_dim; i++)
                permuteRoute(solutionC,tabu);

            tabu.resetTabu();
            std::cout<<"reset nr "<< resetsDone << "/"<<50<< std::endl;
            solutionC_cost = routeCost(solutionC);
        }
        if(resetsDone == 30)
            stopCriteria = true;
        tabu.decrementTenure();
    }
}

void TownsATSP::swapTowns(int* route, int a, int b) {
    int buf = route[a];
    route[a] = route[b];
    route[b] = buf;
}

void TownsATSP::resetSolution(int* solution) {
    for(int j = 0; j < this->map_dim/2; j++)
        permuteRoute(solution);
}

int *TownsATSP::getNeighbourPermutation(int *basePermutation, int size) {

    if(basePermutation == nullptr || size <= 0)
        return nullptr;

    int* neighbourPerm = new int[size];
    memcpy(neighbourPerm, basePermutation, size* sizeof(int) );
    permuteRoute(neighbourPerm);
    return neighbourPerm;
}

void TownsATSP::permuteRoute(int *route, TabuMatrix &tabu) {
    int a, b;
    TabuMatrix localTabu = *new TabuMatrix(map_dim,1);

    a = std::rand()%map_dim;
    do
        b = std::rand()%map_dim;
    while(a == b || tabu.isTabu(route[a],route[b]) || localTabu.isTabu(route[a], route[b]));

    localTabu.addMove(route[a], route[b]);

    int buf = route[a];
    route[a] = route[b];
    route[b] = buf;

}
