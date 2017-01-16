//
// Created by piotrek on 13.01.17.
//

#ifndef TRAVELLINGSALESMAN_SPECIMEN_H
#define TRAVELLINGSALESMAN_SPECIMEN_H
#include <cstdlib>

class Specimen {
private:
    int* chromosome;
    int genesNum;
    double routeCost;
public:
    Specimen(int genesNum);
    ~Specimen(){delete[] chromosome;}

    void decodeChromosome(int* root, int* routeFromChromosome);
    void setChromosome(int* chromosome);
    void getChromosome(int* chromosome);
    int getGenesNum(){ return  this->genesNum;}
    double getCost(){return this->routeCost;}
    void setRouteCost(double cost){this->routeCost = cost;}
    int* getChromosome(){ return this->chromosome;}

};


#endif //TRAVELLINGSALESMAN_SPECIMEN_H
