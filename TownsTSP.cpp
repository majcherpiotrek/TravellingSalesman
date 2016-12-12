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

void TownsTSP::tabuSearch()
{
    /*Wygenerowanie losowego rozwiązania początkowego*/
    int* baseSolution = new int[map_dim];
    memcpy(baseSolution,solution,map_dim*sizeof(int));
    double lowestCost = routeCost(solution);
    TabuList tabu = *(new TabuList(20));
    TabuList::Move* maxMoves = new TabuList::Move[map_dim];

    //Tablicza przechowująca indeksy kolejnych miast w rozwiązaniu
    int* townsPositions = new int[map_dim];
    for (int k = 0; k < map_dim ; ++k)
        townsPositions[baseSolution[k]] = k;

    for(int m = 0; m<5000; m++){
        double currentBestCost = DBL_MAX;
        int bestLeftTown =-1;
        int bestRightTown=-1;
        double diversification = 4;
        //Szukamy najlepszego sąsiada (sąsiedztwo jako losowe map_dim sąsiednich permutacji)
        for (int i = 0; i < diversification*map_dim; ++i) {

            int leftTown;
            int rightTown;
            TabuList::Move move;
            TabuList::Move mirrorMove;
            bool noAspiration = true;
            do {

                do {
                    leftTown = rand() % map_dim;
                    rightTown = rand() % map_dim;
                } while (leftTown == rightTown);

                move = *(new TabuList::Move(leftTown,rightTown));
                mirrorMove = *(new TabuList::Move(rightTown,leftTown));
                //Jeśli ruch jest na tabu
                if(tabu.isOnTheList(move) || tabu.isOnTheList(mirrorMove)){
                    int* tempSolution = new int[map_dim];
                    memcpy(tempSolution, baseSolution, map_dim*sizeof(int));
                    swapTowns(tempSolution, townsPositions[leftTown], townsPositions[rightTown]);
                    double newCost = routeCost(tempSolution);
                    double ratio = (lowestCost - newCost)/lowestCost;
                    ratio *= 100;
                    if(ratio > 0.1){

                        bestLeftTown = leftTown;
                        bestRightTown = rightTown;
                        currentBestCost = newCost;
                        //std::cout<< "aspiracja " << ratio << std::endl;
                        noAspiration = false;
                    }
                    else
                        diversification+=0.5;

                    delete[] tempSolution;
                }
            }while((tabu.isOnTheList(move) || tabu.isOnTheList(mirrorMove)) && noAspiration);

                if(!noAspiration)
                    continue;

                int* tempSolution = new int[map_dim];
                memcpy(tempSolution, baseSolution, map_dim*sizeof(int));
                swapTowns(tempSolution, townsPositions[leftTown], townsPositions[rightTown]);
                double newCost = routeCost(tempSolution);

                //Jeśli ruch daje rozwiązanie lepsze od bazowego
                if(newCost < currentBestCost){
                    bestLeftTown = leftTown;
                    bestRightTown = rightTown;
                    currentBestCost = newCost;
                }

                delete[] tempSolution;
            }

        //wykonujemy ruch
        //std::cout << "\nWykonano ruch: idx: " << townsPositions[bestLeftTown] << " town: " << baseSolution[townsPositions[bestLeftTown]] << std::endl;
        //std::cout << " idx: " << townsPositions[bestRightTown] << " town: " << baseSolution[townsPositions[bestRightTown]] << std::endl;

        //for(int i = 0; i < map_dim; ++i) {
          //  std::cout.flush();
            //std::cout << baseSolution[i] << ", ";
        //}

        //dodajemy go do listy tabu
        if(!tabu.isOnTheList(*new TabuList::Move(bestLeftTown, bestRightTown)) && !tabu.isOnTheList(*new TabuList::Move(bestRightTown, bestLeftTown)))
            tabu.addMove(*(new TabuList::Move(bestLeftTown, bestRightTown)));
        //std::cout << std::endl;
        //std::cout << "tabu list: ";
        //std::cout.flush();
        //tabu.print();

        swapTowns(baseSolution, townsPositions[bestLeftTown], townsPositions[bestRightTown]);
        //Zamieniamy indeksy w tablicy pozycji
        int bufPosition = townsPositions[bestLeftTown];
        townsPositions[bestLeftTown] = townsPositions[bestRightTown];
        townsPositions[bestRightTown] = bufPosition;

        //Jeśli lepsze od aktualnie najlepszego
        if(currentBestCost < lowestCost){
            delete[] solution;
            solution = new int[map_dim];
            memcpy(solution, baseSolution, map_dim*sizeof(int));
            lowestCost = currentBestCost;
        }


    }
}

int* TownsTSP::makeNeighbourPermutation(int* basePermutation, int size, int moveBeg, int moveEnd) {

    if(basePermutation == nullptr)
        return nullptr;

    int* neighbour = new int[size];
    memcpy(neighbour, basePermutation, size* sizeof(int));

    neighbour[moveBeg] = basePermutation[moveEnd];
    neighbour[moveEnd] = basePermutation[moveBeg];

    return neighbour;
}

void TownsTSP::resetSolution() {
    for(int j = 0; j < this->map_dim/2; j++)
        permuteRoute(this->solution);
}

void TownsTSP::resetSolution(int* solution) {
    for(int j = 0; j < this->map_dim/2; j++)
        permuteRoute(solution);
}

void TownsTSP::swapTowns(int* route, int a, int b) {
    int buf = route[a];
    route[a] = route[b];
    route[b] = buf;
}

/*Zwraca zysk z przejscia z rozwiązania A do rozwiązania B*/
double TownsTSP::mval(int *solutionA, int *solutionB) {
    return routeCost(solutionA) - routeCost(solutionB);
}

void TownsTSP::determineMaxMoves(TabuList::Move*& maxMoves, int* positions, int cityCount, TabuList& tabu)
{
    for (int i = 0; i < cityCount; ++i)
    {
        maxMoves[i].leftTown = 0;
        maxMoves[i].rightTown = cityCount - 1;
    }

    for (int i = 0; i < tabu.getCurrentLength(); ++i)
    {
        int tL = tabu.getMove(i).leftTown;
        int tR = tabu.getMove(i).rightTown;
        maxMoves[tL].leftTown = std::min(positions[tL], std::max(positions[tR] + 1, maxMoves[tL].leftTown));
        maxMoves[tR].rightTown = std::max(positions[tR], std::min(positions[tL] - 1, maxMoves[tR].rightTown));
    }
}
















































