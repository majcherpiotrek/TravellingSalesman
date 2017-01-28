//
// Created by piotrek on 13.01.17.
//

#include <cstring>
#include <cfloat>
#include "Specimen.h"

Specimen::Specimen(int genesNum) {
    this->genesNum = genesNum;
    this->chromosome = new int[genesNum];
    this->routeCost = DBL_MAX;
}

void Specimen::decodeChromosome(int* root, int* routeFromChromosome) {

    bool used[genesNum];
    for (int i = 0; i < genesNum; ++i)
        used[i] = false;

    for (int i = 0; i < genesNum ; ++i) {
        int k = -1;
        int j = 0;
        int index = this->chromosome[i];

        while (k<index){
            if(!used[j])
                k++;
            if(k == index)
                break;
            j++;
        }

        routeFromChromosome[i] = root[j];
        used[j] = true;
    }

}

void Specimen::setChromosome(int *chromosome) {
    memcpy(this->chromosome, chromosome, this->genesNum*sizeof(int));
}

void Specimen::getChromosome(int * chromosome) {
    memcpy(chromosome, this->chromosome, this->genesNum*sizeof(int));
}


