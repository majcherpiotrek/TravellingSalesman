#include "Menu.h"
#include "TownsTSP.h"
#include "TownsATSP.h"

int main()
{
    Menu main_menu = *(new Menu());

    main_menu.add("PIOTR MAJCHER - PROJEKT PEA - PROBLEM KOMIWOJAZERA\n");
    main_menu.add("MENU GLOWNE\n");
    main_menu.add("***************************************************************************\n\n(r) Odswiez widok menu.\n");
    main_menu.add("(a) Symulowane wyzarzanie - ATSP\n");
    main_menu.add("(t) Symulowane wyzarzanie - TSP\n");
    main_menu.add("(b) Tabu search - ATSP\n");
    main_menu.add("(c) Tabu search - TSP\n");

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

            case 'a':
            {
                std::cout << "\nInstancje problemu ATSP:\nftv35.atsp\nft70.atsp\nftv170.atsp\nWybierz jeden z plikow i podaj jego nazwe razem z rozszerzeniem: ";
                std::string nazwa;
                std::cin >> nazwa;

                TownsATSP atsp = *(new TownsATSP());
                if(atsp.loadMap(nazwa))
                {
                    std::cout << std::endl << std::endl;
                    double wynik;
                    atsp.performSA();
                    wynik = atsp.routeCost(atsp.solution);

                    std::cout << "Koszt znalezionego rozwiazania: " << wynik << std::endl;
                    std::cout << "Koszt rozwiazania optymalnego: " << atsp.getOptCost() << std::endl;
                }
                break;
            }

            case 't':
            {
                std::cout << "\nInstancje problemu TSP:\nberlin52.tsp\nch130.tsp\na280.tsp\nWybierz jeden z plikow i podaj jego nazwe razem z rozszerzeniem: ";
                std::string nazwa;
                std::cin >> nazwa;

                TownsTSP tsp = *(new TownsTSP());
                if(tsp.loadMap(nazwa))
                {
                    std::cout << std::endl << std::endl;
                    double wynik;
                    tsp.performSA();
                    wynik = tsp.routeCost(tsp.solution);

                    std::cout << "Koszt znalezionego rozwiazania: " << wynik << std::endl;
                    std::cout << "Koszt rozwiazania optymalnego: " << tsp.getOptCost() << std::endl;
                }

                break;
            }

            case 'b':
            {
                std::cout << "\nInstancje problemu ATSP:\nftv35.atsp\nft70.atsp\nftv170.atsp\nWybierz jeden z plikow i podaj jego nazwe razem z rozszerzeniem: ";
                std::string nazwa;
                std::cin >> nazwa;

                TownsATSP atsp = *(new TownsATSP());
                if(atsp.loadMap(nazwa))
                {
                    std::cout << std::endl << std::endl;
                    double wynik;
                    atsp.tabuSearch(1000);
                    wynik = atsp.routeCost(atsp.solution);

                    std::cout << "Koszt znalezionego rozwiazania: " << wynik << std::endl;
                    std::cout << "Koszt rozwiazania optymalnego: " << atsp.getOptCost() << std::endl;
                }
                break;
            }

            case 'c':
            {
                std::cout << "\nInstancje problemu TSP:\nberlin52.tsp\nch130.tsp\na280.tsp\nWybierz jeden z plikow i podaj jego nazwe razem z rozszerzeniem: ";
                std::string nazwa;
                std::cin >> nazwa;

                TownsTSP tsp = *(new TownsTSP());
                if(tsp.loadMap(nazwa))
                {
                    std::cout << std::endl << std::endl;
                    double wynik;
                    tsp.tabuSearch(1000);
                    wynik = tsp.routeCost(tsp.solution);

                    std::cout << "Koszt znalezionego rozwiazania: " << wynik << std::endl;
                    std::cout << "Koszt rozwiazania optymalnego: " << tsp.getOptCost() << std::endl;
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