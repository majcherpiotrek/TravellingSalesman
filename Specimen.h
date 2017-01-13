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
public:
    Specimen(int genesNum);
    ~Specimen(){if(chromosome != nullptr)delete[] chromosome;}

    void decodeChromosome(int&* root, int&* routeFromChromosome);
    void setChromosome(int* chromosome);
    void getChromosome(int&* chromosome);


};


#endif //TRAVELLINGSALESMAN_SPECIMEN_H
