//
// Created by piotrek on 01.12.16.
//

#include "TownsTSP.h"

TownsTSP::TownsTSP()
{
    problem_name = "";
    map_dim = 0;
    coords_list = nullptr;
    towns_map = nullptr;
    solution = nullptr;
}

TownsTSP::~TownsTSP()
{
    delete[] coords_list;
    delete[] solution;
    delete[] towns_map;
}

bool TownsTSP::loadMap(std::string file_name)
{
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

int TownsTSP::getOptCost()
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

double TownsTSP::computeDistance(coords A, coords B)
{
    double xd = B.x - A.x;
    double yd = B.y - A.y;

    double dist = sqrt(xd*xd + yd*yd);

    return dist;
}

double TownsTSP::routeCost(int *route)
{
    int cost = 0;

    for( int i = 0; i+1 < map_dim; i++)
        cost += towns_map[ route[i] ][ route[i+1] ];

    return cost;
}

void TownsTSP::permuteRoute(int* route)
{
    int a, b;

    a = std::rand()%map_dim;
    do
        b = std::rand()%map_dim;
    while(a == b);


    int buf = route[a];
    route[a] = route[b];
    route[b] = buf;
}

void TownsTSP::permuteRoute(int* route, TabuMatrix& tabu)
{
    int a, b;

    TabuMatrix localTabu = *new TabuMatrix(map_dim,1);

    a = std::rand()%map_dim;
    do
        b = std::rand()%map_dim;
    while(a == b || tabu.isTabu(route[a],route[b]) || localTabu.isTabu(route[a], route[b]));

    localTabu.addMove(route[a], route[b]);
    localTabu.addMove(route[b], route[a]);

    int buf = route[a];
    route[a] = route[b];
    route[b] = buf;


}

void TownsTSP::performSA()
{

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

void TownsTSP::deterministicTabu(int tt, int it)
{
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

        if(solutionC_cost - neighbourhoodLowest < 20)
            iterations++;

        /*add the move to tabu*/
        tabu.addMove(solutionC[bestLeft], solutionC[bestRight]);
        tabu.addMove(solutionC[bestRight], solutionC[bestLeft]);
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
                std::cout <<"poprawa po resecie TSP nr" << resetsDone << std::endl;
        }


        //std::cout<< iterations << std::endl;
        if(iterations == it){
            resetsDone++;
            iterations = 0;
            delete[] solutionC;
            solutionC = new int [map_dim];
            memcpy(solutionC, solution, map_dim*sizeof(int));
            for(int i = 0; i < 10; i++)
                permuteRoute(solutionC,tabu);

            //tabu.resetTabu();
            std::cout<<"reset nr "<< resetsDone << "/"<<30<< std::endl;
            solutionC_cost = routeCost(solutionC);

        }
        if(resetsDone == 5)
            stopCriteria = true;
        tabu.decrementTenure();
    }
    //std::cout<<"iteracje: " <<iterations <<std::endl;
}


void TownsTSP::resetSolution() {
    for(int j = 0; j < this->map_dim; j++)
        permuteRoute(this->solution);
}

void TownsTSP::resetSolution(int* solution) {
    for(int j = 0; j < this->map_dim; j++)
        permuteRoute(solution);
}

void TownsTSP::swapTowns(int* route, int a, int b) {
    int buf = route[a];
    route[a] = route[b];
    route[b] = buf;
}

void TownsTSP::randomNeighbourhoodTabu(int tt)
{
    /*random initial solution*/
    int* solutionC = new int [map_dim];
    //for (int k = 0; k < map_dim ; ++k)
    //solutionC[k] = k;
    memcpy(solutionC,solution, map_dim*sizeof(int));

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

        //neighbourhood size
        int nSize = (int)(0.5*map_dim*(map_dim-1));
        //local Tabu for searching neighbours
        TabuMatrix* localTabu = new TabuMatrix(map_dim,1);
        //counter for improvement in searching the neighbourhood (how many times we improved the result)
        int improvementCounter = 0;
        for (int i = 0; i < nSize && improvementCounter < 10*map_dim; ++i) {

                int a,b;
                do{
                    a=rand()%map_dim;
                    b=rand()%map_dim;
                }while( a==b || localTabu->isTabu(a, b));

                //we forbid these moves so we get new random moves from the neighbourhood
                localTabu->addMove(a,b);
                localTabu->addMove(b,a);

                /*we check aspiration*/
                if(tabu.isTabu(solutionC[a], solutionC[b])){
                    swapTowns(solutionC, a, b);
                    /*calculating the cost of neighbour solution*/
                    double tempCost = routeCost(solutionC);
                    /*if move on tabu leads to solution better then ever*/
                    if(tempCost <= globalLowest && tempCost <= neighbourhoodLowest){
                        neighbourhoodLowest = tempCost;
                        bestLeft = a;
                        bestRight = b;
                    }
                    /*come back to initial solution*/
                    swapTowns(solutionC, a, b);
                    continue;
                }

                swapTowns(solutionC, a, b);
                /*calculating the cost of neighbour solution*/
                double tempCost = routeCost(solutionC);


                /*if the move leads to the currently best solution in neighbourhood then save it as currently best*/
                if (tempCost <= neighbourhoodLowest) {
                    neighbourhoodLowest = tempCost;
                    bestLeft = a;
                    bestRight = b;

                }
                else
                    improvementCounter++;
                /*come back to initial solution*/
                swapTowns(solutionC, a, b);
        }

        if(solutionC_cost - neighbourhoodLowest < 10)
            iterations++;

        /*add the move to tabu*/
        tabu.addMove(solutionC[bestLeft], solutionC[bestRight]);
        tabu.addMove(solutionC[bestRight], solutionC[bestLeft]);

        swapTowns(solutionC, bestLeft, bestRight);
        solutionC_cost = neighbourhoodLowest;


        /*if solutionC is now better then global best solution, then update the global best*/
        if (solutionC_cost < globalLowest) {
            delete[] solution;
            solution = new int[map_dim];
            memcpy(solution, solutionC, map_dim * sizeof(int));
            globalLowest = solutionC_cost;
            if(resetsDone > 0)
                std::cout <<"poprawa po resecie TSP nr" << resetsDone << std::endl;
        }


        //std::cout<< iterations << std::endl;
        if(iterations == 20){
            resetsDone++;
            iterations = 0;

            for(int i = 0; i < 2*map_dim; i++)
                permuteRoute(solutionC,tabu);

            tabu.resetTabu();
            std::cout<<"reset nr "<< resetsDone << "/"<<50 << std::endl;
            solutionC_cost = routeCost(solutionC);

        }
        if(resetsDone == 50)
            stopCriteria = true;
        tabu.decrementTenure();
    }
    //std::cout<<"iteracje: " <<iterations <<std::endl;
}

















































