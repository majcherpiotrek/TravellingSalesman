//
// Created by piotrek on 01.12.16.
//

#include "TownsTSP.h"
#include "TownsATSP.h"
#include <ctime>

void run_tests_SA(void)
{
    int n = 4;
    double t_rate_tab[] = { 0.85, 0.90, 0.99, 0.999};

    int n_files = 6;
    std::string files[] = {"berlin52.tsp", "ch130.tsp", "a280.tsp","ftv35.atsp", "ft70.atsp", "ftv170.atsp"};
    std::string res_files_time[] = {"berlin52_res.csv", "ch130_res.csv", "a280_res.csv", "ftv35_res.csv", "ft70_res.csv", "ftv170_res.csv"};
    std::string res_files_MSTK[] = {"berlin52_res_MSTK.csv", "ch130_res_MSTK.csv", "a280_res_MSTK.csv", "ftv35_res_MSTK.csv", "ft70_res_MSTK.csv", "ftv170_res_MSTK.csv"};

    TownsATSP atsp = *(new TownsATSP());
    TownsTSP tsp = *(new TownsTSP());

    double* opt_sol = new double[n_files];




    int repetitions = 100;

    for(int i = 0; i <  n_files; i++)
    {
        double* time_results = new double[n];
        double* mistake = new double[n];
        double* costs = new double[n];

        if( i >= 3) //ATSP
        {
            TownsATSP prob = *(new TownsATSP());
            prob.loadMap(files[i]);
            opt_sol[i] = (double)prob.getOptCost();
            for(int k = 0; k < n; k++) //Przechodzimy przez wszystkie wartości t_rate
            {
                double t = 0;
                double mstk = 0;
                double cst = 0;
                for(int n = 0; n < repetitions; n++) //Powtarzamy repetitions razy dla uśrednienia
                {
                    clock_t START, STOP;

                    START = clock();
                    prob.performSA(/*t_rate_tab[k]*/);
                    STOP = clock();

                    t += (double)(STOP-START)/CLOCKS_PER_SEC*1000;

                    double x = prob.routeCost(prob.solution);
                    cst += x;
                    mstk += x - opt_sol[i];

                    prob.resetSolution();
                }

                /*Liczymy średni czas i dodajemy do tablicy wyników*/
                time_results[k] =  t/repetitions;
                mistake[k] = 100*(mstk/repetitions)/opt_sol[i];
                costs[k] = cst/repetitions;
            }

            /*Zapisanie wyników dla danej instancji do pliku*/
            std::fstream plik;
            std::fstream plik_mstk;

            plik.open(res_files_time[i], std::ios::out);
            plik_mstk.open(res_files_MSTK[i], std::ios::out);

            for (int j = 0; j < n; j++)
            {
                plik << time_results[j] << ";";
                plik_mstk << mistake[j] << ";";
            }
            for (int j = 0; j < n; j++)
                plik_mstk << costs[j] << ";";


            plik.close();
            plik_mstk.close();
        }
        else //TSP
        {
            TownsTSP prob = *(new TownsTSP());
            prob.loadMap(files[i]);
            opt_sol[i] = (double)prob.getOptCost();

            for(int k = 0; k < n; k++) //Przechodzimy przez wszystkie wartości t_rate
            {
                double t = 0;
                double mstk = 0;
                double cst = 0;

                for(int n = 0; n < repetitions; n++) //Powtarzamy repetitions razy dla uśrednienia
                {
                    clock_t START, STOP;

                    START = clock();
                    prob.performSA(/*t_rate_tab[k]*/);
                    STOP = clock();

                    t += (double)(STOP-START)/CLOCKS_PER_SEC*1000;

                    double x = prob.routeCost(prob.solution);
                    cst += x;
                    mstk += x - opt_sol[i];


                        prob.resetSolution();
                }

                /*Liczymy średni czas i dodajemy do tablicy wyników*/
                time_results[k] =  t/repetitions;
                mistake[k] = 100*(mstk/repetitions)/opt_sol[i];
                costs[k] = cst/repetitions;
            }

            /*Zapisanie wyników dla danej instancji do pliku*/
            std::fstream plik;
            std::fstream plik_mstk;

            plik.open(res_files_time[i], std::ios::out);
            plik_mstk.open(res_files_MSTK[i], std::ios::out);

            for (int j = 0; j < n; j++)
            {
                plik << time_results[j] << ";";
                plik_mstk << mistake[j] << ";";
            }
            for (int j = 0; j < n; j++)
                plik_mstk << costs[j] << ";";

            plik.close();
            plik_mstk.close();

        }

        delete[] time_results;
    }

    return;
}
void run_tests_TS(void)
{
    int n = 4;
    double t_rate_tab[] = { 0.85, 0.90, 0.99, 0.999};

    int n_files = 6;
    std::string files[] = {"berlin52.tsp", "ch130.tsp", "a280.tsp","ftv35.atsp", "ft70.atsp", "ftv170.atsp"};
    std::string res_files_time[] = {"berlin52_res.csv", "ch130_res.csv", "a280_res.csv", "ftv35_res.csv", "ft70_res.csv", "ftv170_res.csv"};
    std::string res_files_MSTK[] = {"berlin52_res_MSTK.csv", "ch130_res_MSTK.csv", "a280_res_MSTK.csv", "ftv35_res_MSTK.csv", "ft70_res_MSTK.csv", "ftv170_res_MSTK.csv"};

    TownsATSP atsp = *(new TownsATSP());
    TownsTSP tsp = *(new TownsTSP());

    double* opt_sol = new double[n_files];




    int repetitions = 100;

    for(int i = 0; i <  n_files; i++)
    {
        double* time_results = new double[n];
        double* mistake = new double[n];
        double* costs = new double[n];

        if( i >= 3) //ATSP
        {
            TownsATSP prob = *(new TownsATSP());
            prob.loadMap(files[i]);
            opt_sol[i] = (double)prob.getOptCost();
            for(int k = 0; k < n; k++) //Przechodzimy przez wszystkie wartości t_rate
            {
                double t = 0;
                double mstk = 0;
                double cst = 0;
                for(int n = 0; n < repetitions; n++) //Powtarzamy repetitions razy dla uśrednienia
                {
                    clock_t START, STOP;

                    START = clock();
                    prob.performSA(/*t_rate_tab[k]*/);
                    STOP = clock();

                    t += (double)(STOP-START)/CLOCKS_PER_SEC*1000;

                    double x = prob.routeCost(prob.solution);
                    cst += x;
                    mstk += x - opt_sol[i];

                    prob.resetSolution();
                }

                /*Liczymy średni czas i dodajemy do tablicy wyników*/
                time_results[k] =  t/repetitions;
                mistake[k] = 100*(mstk/repetitions)/opt_sol[i];
                costs[k] = cst/repetitions;
            }

            /*Zapisanie wyników dla danej instancji do pliku*/
            std::fstream plik;
            std::fstream plik_mstk;

            plik.open(res_files_time[i], std::ios::out);
            plik_mstk.open(res_files_MSTK[i], std::ios::out);

            for (int j = 0; j < n; j++)
            {
                plik << time_results[j] << ";";
                plik_mstk << mistake[j] << ";";
            }
            for (int j = 0; j < n; j++)
                plik_mstk << costs[j] << ";";


            plik.close();
            plik_mstk.close();
        }
        else //TSP
        {
            TownsTSP prob = *(new TownsTSP());
            prob.loadMap(files[i]);
            opt_sol[i] = (double)prob.getOptCost();

            for(int k = 0; k < n; k++) //Przechodzimy przez wszystkie wartości t_rate
            {
                double t = 0;
                double mstk = 0;
                double cst = 0;

                for(int n = 0; n < repetitions; n++) //Powtarzamy repetitions razy dla uśrednienia
                {
                    clock_t START, STOP;

                    START = clock();
                    prob.performSA(/*t_rate_tab[k]*/);
                    STOP = clock();

                    t += (double)(STOP-START)/CLOCKS_PER_SEC*1000;

                    double x = prob.routeCost(prob.solution);
                    cst += x;
                    mstk += x - opt_sol[i];


                    prob.resetSolution();
                }

                /*Liczymy średni czas i dodajemy do tablicy wyników*/
                time_results[k] =  t/repetitions;
                mistake[k] = 100*(mstk/repetitions)/opt_sol[i];
                costs[k] = cst/repetitions;
            }

            /*Zapisanie wyników dla danej instancji do pliku*/
            std::fstream plik;
            std::fstream plik_mstk;

            plik.open(res_files_time[i], std::ios::out);
            plik_mstk.open(res_files_MSTK[i], std::ios::out);

            for (int j = 0; j < n; j++)
            {
                plik << time_results[j] << ";";
                plik_mstk << mistake[j] << ";";
            }
            for (int j = 0; j < n; j++)
                plik_mstk << costs[j] << ";";

            plik.close();
            plik_mstk.close();

        }

        delete[] time_results;
    }

    return;
}

int test_program(void)
{
    run_tests_SA();
    return 0;
}