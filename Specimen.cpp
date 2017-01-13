//
// Created by piotrek on 13.01.17.
//

#include <cstring>
#include "Specimen.h"

Specimen::Specimen(int genesNum) {
    this->genesNum = genesNum;
    this->chromosome = new int[genesNum];
}

void Specimen::decodeChromosome(int&* root, int&* routeFromChromosome) {
    //tablica przechowujące informację o nr wierzchołków z root,
    //które zostały już dodane do routeDFromChromosome
    bool usedIndexes[genesNum];
    for(int i = 0; i < genesNum; i++)
        usedIndexes[i] = false;

    for (int i = 0; i < genesNum; ++i) {
        int index = this->chromosome[i];
        while(usedIndexes[index])
            index++;
        routeFromChromosome[i] = root[index];
        usedIndexes[index] = true;
    }
}

void Specimen::setChromosome(int *chromosome) {
    memcpy(this->chromosome, chromosome, this->genesNum*sizeof(int));
}

void Specimen::getChromosome(int &*chromosome) {
    memcpy(chromosome, this->chromosome, this->genesNum*sizeof(int));
}


