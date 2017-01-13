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
    //tablica przechowujące informację o nr wierzchołków z root,
    //które zostały już dodane do routeDFromChromosome
    bool usedIndexes[genesNum];
    for(int i = 0; i < genesNum; i++)
        usedIndexes[i] = false;

    for (int i = 0; i < genesNum; ++i) {
        //zapisujemy kolejny indeks porządkowy z chromosomu
        int index = this->chromosome[i];
        //szukamy aktualnego początku
        int currentBeg = 0;
        while(usedIndexes[currentBeg])
            currentBeg++;

        //dodajemy do ineksu porządkowego aktualny początek
        index += currentBeg;
        //jeśli kolejne indeksy były wykorzystane to szukamy pierwszego niewykorzystanego
        while(usedIndexes[index])
            index++;

        //dodajemy do rozkodowanego rozwiązania nr miasta z tabeli wyjściowej
        routeFromChromosome[i] = root[index];
        usedIndexes[index] = true;
    }
}

void Specimen::setChromosome(int *chromosome) {
    memcpy(this->chromosome, chromosome, this->genesNum*sizeof(int));
}

void Specimen::getChromosome(int * chromosome) {
    memcpy(chromosome, this->chromosome, this->genesNum*sizeof(int));
}


