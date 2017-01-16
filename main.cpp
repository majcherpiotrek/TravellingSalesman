#include "Menu.h"
#include "TownsTSP.h"
#include "TownsATSP.h"
#include <ctime>

int ain()
{
    Menu main_menu = *(new Menu());

    main_menu.add("PIOTR MAJCHER - PROJEKT PEA - PROBLEM KOMIWOJAZERA\n");
    main_menu.add("MENU GLOWNE\n");
    main_menu.add("***************************************************************************\n\n(r) Odswiez widok menu.\n");
    main_menu.add("(1) Symulowane wyzarzanie - ATSP\n");
    main_menu.add("(2) Symulowane wyzarzanie - TSP\n");
    main_menu.add("(3) Tabu search deterministyczny - ATSP\n");
    main_menu.add("(4) Tabu search deterministyczny - TSP\n");
    main_menu.add("(5) Tabu search losowe sasiedztwo- ATSP\n");
    main_menu.add("(6) Tabu search losowe sasiedztwo - TSP\n");
    main_menu.add("(7) Algorytm genetyczny - TSP\n");
    main_menu.add("(8) Algorytm genetyczny - ATSP\n");

    main_menu.add("\n(k) Zakoncz program ... ");

    bool end = false;
    bool bad_command = false;

    std::cout << main_menu;

    while(!end)
    {
        if (bad_command)
        {
            std::cout << "Bledny numer opcji! Twoj wybor-> ";
            bad_command = false;
        }
        else
            std::cout << "\nTwoj wybor -> ";

        std::cin.clear();
        std::cin.sync();

        char decision;
        std::cin >> decision;

        switch(decision)
        {
            case 'r':
            {
                //odswiez ekran
                system("clear");
                std::cout << main_menu;
                break;
            }

            case '1':
            {
                std::cout << "\nInstancje problemu ATSP:\nftv35.atsp\nft70.atsp\nftv170.atsp\nWybierz jeden z plikow i podaj jego nazwe razem z rozszerzeniem: ";
                std::string nazwa;
                std::cin >> nazwa;

                TownsATSP atsp = *(new TownsATSP());
                if(atsp.loadMap(nazwa))
                {
                    double optCost = atsp.getOptCost();
                    clock_t START, STOP;
                    double t;
                    std::cout << std::endl << std::endl;
                    double wynik;

                    START=clock();
                    atsp.performSA();
                    STOP=clock();

                    t = (double)(STOP - START)/CLOCKS_PER_SEC*1000;
                    wynik = atsp.routeCost(atsp.solution);

                    std::cout << "Koszt znalezionego rozwiazania: " << wynik << std::endl;
                    std::cout << "Koszt rozwiazania optymalnego: " << optCost << std::endl;
                    std::cout << "Błąd: " << 100*((wynik-optCost)/optCost) << "%" << std::endl;
                    std::cout << "Czas wykonania: " << t << " ms" << std::endl;
                }
                break;
            }

            case '2':
            {
                std::cout << "\nInstancje problemu TSP:\nberlin52.tsp\nch130.tsp\na280.tsp\nWybierz jeden z plikow i podaj jego nazwe razem z rozszerzeniem: ";
                std::string nazwa;
                std::cin >> nazwa;

                TownsTSP tsp = *(new TownsTSP());
                if(tsp.loadMap(nazwa))
                {
                    double optCost = tsp.getOptCost();
                    clock_t START, STOP;
                    double t;
                    std::cout << std::endl << std::endl;
                    double wynik;

                    START = clock();
                    tsp.performSA();
                    STOP = clock();

                    t = (double)(STOP - START)/CLOCKS_PER_SEC*1000;
                    wynik = tsp.routeCost(tsp.solution);

                    std::cout << "Koszt znalezionego rozwiazania: " << wynik << std::endl;
                    std::cout << "Koszt rozwiazania optymalnego: " << optCost << std::endl;
                    std::cout << "Błąd: " << 100*((wynik-optCost)/optCost) << "%" << std::endl;
                    std::cout << "Czas wykonania: " << t << " ms" << std::endl;
                }

                break;
            }

            case '3':
            {
                std::cout << "\nInstancje problemu ATSP:\nftv35.atsp\nft70.atsp\nftv170.atsp\nWybierz jeden z plikow i podaj jego nazwe razem z rozszerzeniem: ";
                std::string nazwa;
                std::cin >> nazwa;

                TownsATSP atsp = *(new TownsATSP());
                if(atsp.loadMap(nazwa))
                {
                    double optCost = atsp.getOptCost();
                    clock_t START, STOP;
                    double t;
                    std::cout << std::endl << std::endl;
                    std::cout << "Kadencja 1.5: " << (int)(1.5*atsp.getSize()) << std::endl;
                    std::cout << "Kryterium resetu (iteracje): 400" <<std::endl;
                    std::cout << "Kryterium zatrzymania (liczba resetów) : 5" << std::endl;
                    double wynik;
                    START =clock();
                    atsp.deterministicTabu((int)(1.5*atsp.getSize()), 400);
                    STOP =clock();

                    t = (double)(STOP - START)/CLOCKS_PER_SEC*1000;
                    wynik = atsp.routeCost(atsp.solution);

                    std::cout << "Koszt znalezionego rozwiazania: " << wynik << std::endl;
                    std::cout << "Koszt rozwiazania optymalnego: " << optCost << std::endl;
                    std::cout << "Błąd: " << 100*((wynik-optCost)/optCost) << "%" << std::endl;
                    std::cout << "Czas wykonania: " << t << " ms" << std::endl;
                }
                break;
            }

            case '4':
            {
                std::cout << "\nInstancje problemu TSP:\nberlin52.tsp\nch130.tsp\na280.tsp\nWybierz jeden z plikow i podaj jego nazwe razem z rozszerzeniem: ";
                std::string nazwa;
                std::cin >> nazwa;

                TownsTSP tsp = *(new TownsTSP());
                if(tsp.loadMap(nazwa))
                {
                    double optCost = tsp.getOptCost();
                    clock_t START, STOP;
                    double t;
                    std::cout << std::endl << std::endl;
                    std::cout << "Kadencja 1.5: " << (int)(1.5*tsp.getSize()) << std::endl;
                    std::cout << "Kryterium resetu (iteracje): 400" <<std::endl;
                    std::cout << "Kryterium zatrzymania (liczba resetów) : 5" << std::endl;
                    double wynik;
                    START =clock();
                    tsp.deterministicTabu((int)(1.5*tsp.getSize()), 400);
                    STOP = clock();

                    t = (double)(STOP - START)/CLOCKS_PER_SEC*1000;
                    wynik = tsp.routeCost(tsp.solution);

                    std::cout << "Koszt znalezionego rozwiazania: " << wynik << std::endl;
                    std::cout << "Koszt rozwiazania optymalnego: " << optCost << std::endl;
                    std::cout << "Błąd: " << 100*((wynik-optCost)/optCost) << "%" << std::endl;
                    std::cout << "Czas wykonania: " << t << " ms" << std::endl;
                }

                break;
            }

            case '5':
            {
                std::cout << "\nInstancje problemu ATSP:\nftv35.atsp\nft70.atsp\nftv170.atsp\nWybierz jeden z plikow i podaj jego nazwe razem z rozszerzeniem: ";
                std::string nazwa;
                std::cin >> nazwa;

                TownsATSP atsp = *(new TownsATSP());
                if(atsp.loadMap(nazwa))
                {
                    double optCost = atsp.getOptCost();
                    clock_t START, STOP;
                    double t;
                    std::cout << std::endl << std::endl;
                    std::cout << "Kadencja 1.5: " << (int)(1.5*atsp.getSize()) << std::endl;
                    std::cout << "Kryterium resetu (iteracje): 600" <<std::endl;
                    std::cout << "Kryterium zatrzymania (liczba resetów) : 5" << std::endl;
                    double wynik;
                    START =clock();
                    atsp.randomNeighbourhoodTabu((int)(1.5*atsp.getSize()), 600, 3*atsp.getSize());
                    STOP =clock();

                    t = (double)(STOP - START)/CLOCKS_PER_SEC*1000;
                    wynik = atsp.routeCost(atsp.solution);

                    std::cout << "Koszt znalezionego rozwiazania: " << wynik << std::endl;
                    std::cout << "Koszt rozwiazania optymalnego: " << optCost << std::endl;
                    std::cout << "Błąd: " << 100*((wynik-optCost)/optCost) << "%" << std::endl;
                    std::cout << "Czas wykonania: " << t << " ms" << std::endl;
                }
                break;
            }

            case '6':
            {
                std::cout << "\nInstancje problemu TSP:\nberlin52.tsp\nch130.tsp\na280.tsp\nWybierz jeden z plikow i podaj jego nazwe razem z rozszerzeniem: ";
                std::string nazwa;
                std::cin >> nazwa;

                TownsTSP tsp = *(new TownsTSP());
                if(tsp.loadMap(nazwa))
                {
                    double optCost = tsp.getOptCost();
                    clock_t START, STOP;
                    double t;
                    std::cout << std::endl << std::endl;
                    std::cout << "Kadencja 1.5: " << (int)(1.5*tsp.getSize()) << std::endl;
                    std::cout << "Kryterium resetu (iteracje): 600" <<std::endl;
                    std::cout << "Kryterium zatrzymania (liczba resetów) : 5" << std::endl;
                    double wynik;
                    START =clock();
                    tsp.randomNeighbourhoodTabu((int)(1.5*tsp.getSize()), 600, 3*tsp.getSize());
                    STOP = clock();

                    t = (double)(STOP - START)/CLOCKS_PER_SEC*1000;
                    wynik = tsp.routeCost(tsp.solution);

                    std::cout << "Koszt znalezionego rozwiazania: " << wynik << std::endl;
                    std::cout << "Koszt rozwiazania optymalnego: " << optCost << std::endl;
                    std::cout << "Błąd: " << 100*((wynik-optCost)/optCost) << "%" << std::endl;
                    std::cout << "Czas wykonania: " << t << " ms" << std::endl;
                }

                break;
            }
            case '7':
            {
                std::cout << "\nInstancje problemu TSP:\nberlin52.tsp\nch130.tsp\na280.tsp\nWybierz jeden z plikow i podaj jego nazwe razem z rozszerzeniem: ";
                std::string nazwa;
                std::cin >> nazwa;

                TownsTSP tsp = *(new TownsTSP());
                if(tsp.loadMap(nazwa))
                {
                    double optCost = tsp.getOptCost();
                    clock_t START, STOP;
                    double t;
                    std::cout << std::endl << std::endl;
                    std::cout << "Elitaryzm 10%: " << (int)(0.1*tsp.getSize()) << " osobnikow w kazdej populacji" std::endl;
                    std::cout << "Liczba pokolen: 200" <<std::endl;
                    std::cout << "Rozmiar populacji: 2000" << std::endl;
                    std::cout << "Prawdopodobienstwo mutacji: 0.1%" << std::endl;
                    std::cout << "Maksymalny zakres mutacji: 10% chromosomu" <<std::endl;
                    std::cout << "Strategia selekcji: ruletka" <<std::endl;
                    std::cout << "Sposob krzyzowania: uniform crossover" <<std::endl;

                    double wynik;
                    START =clock();
                    tsp.genetic(200,2000,0.1,SelectionStrategy::ROULETTE, CrossoverStrategy::RANDOM);
                    STOP = clock();

                    t = (double)(STOP - START)/CLOCKS_PER_SEC*1000;
                    wynik = tsp.routeCost(tsp.solution);

                    std::cout << "Koszt znalezionego rozwiazania: " << wynik << std::endl;
                    std::cout << "Koszt rozwiazania optymalnego: " << optCost << std::endl;
                    std::cout << "Błąd: " << 100*((wynik-optCost)/optCost) << "%" << std::endl;
                    std::cout << "Czas wykonania: " << t << " ms" << std::endl;
                }

                break;
            }
            case '8':
            {
                std::cout << "\nInstancje problemu ATSP:\nftv35.atsp\nft70.atsp\nftv170.atsp\nWybierz jeden z plikow i podaj jego nazwe razem z rozszerzeniem: ";
                std::string nazwa;
                std::cin >> nazwa;

                TownsATSP atsp = *(new TownsATSP());
                if(atsp.loadMap(nazwa))
                {
                    double optCost = atsp.getOptCost();
                    clock_t START, STOP;
                    double t;
                    std::cout << std::endl << std::endl;
                    std::cout << "Elitaryzm 10%: " << (int)(0.1*atsp.getSize()) << " osobnikow w kazdej populacji" std::endl;
                    std::cout << "Liczba pokolen: 200" <<std::endl;
                    std::cout << "Rozmiar populacji: 2000" << std::endl;
                    std::cout << "Prawdopodobienstwo mutacji: 0.1%" << std::endl;
                    std::cout << "Maksymalny zakres mutacji: 10% chromosomu" <<std::endl;
                    std::cout << "Strategia selekcji: ruletka" <<std::endl;
                    std::cout << "Sposob krzyzowania: uniform crossover" <<std::endl;

                    double wynik;
                    START =clock();
                    atsp.genetic(200,2000,0.1,SelectionStrategy::ROULETTE, CrossoverStrategy::RANDOM);
                    STOP = clock();

                    t = (double)(STOP - START)/CLOCKS_PER_SEC*1000;
                    wynik = atsp.routeCost(atsp.solution);

                    std::cout << "Koszt znalezionego rozwiazania: " << wynik << std::endl;
                    std::cout << "Koszt rozwiazania optymalnego: " << optCost << std::endl;
                    std::cout << "Błąd: " << 100*((wynik-optCost)/optCost) << "%" << std::endl;
                    std::cout << "Czas wykonania: " << t << " ms" << std::endl;
                }

                break;
            }

            case 'k':
            {
                end = true;
                break;
            }

            default:
                bad_command = true;
        }
    }

    return 0;
}