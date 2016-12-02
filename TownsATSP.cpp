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
    while(a == b);


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

void TownsATSP::tabuSearch(int iterations) {

    /*Rozwiązanie początkowe*/
    double currentLowestRouteCost = routeCost(this->solution);

    int* baseSolution = new int[this->map_dim];
    memcpy(baseSolution, this->solution, this->map_dim* sizeof(int));
    double baseCost = routeCost(baseSolution);

    /*Losujemy sąsiada rozwiązania początkowego*/
    for(int i=0; i<iterations; i++) {
        int *neighbourSolution = getNeighbourPermutation(baseSolution, this->map_dim);

        double neighbourCost = routeCost(neighbourSolution);

        if (neighbourCost < baseCost) {
            delete[] baseSolution;
            baseSolution = neighbourSolution;
            baseCost = neighbourCost;
        }

        if (baseCost < currentLowestRouteCost) {
            delete[] this->solution;
            this->solution = new int[this->map_dim];
            memcpy(this->solution, baseSolution, this->map_dim*sizeof(int));
            currentLowestRouteCost = baseCost;
        }
    }
}

int *TownsATSP::getNeighbourPermutation(int *basePermutation, int size) {

    if(basePermutation == nullptr || size <= 0)
        return nullptr;

    int* neighbourPerm = new int[size];
    memcpy(neighbourPerm, basePermutation, size* sizeof(int) );
    permuteRoute(neighbourPerm);
    return neighbourPerm;
}
