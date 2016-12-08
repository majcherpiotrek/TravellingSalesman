//
// Created by piotrek on 01.12.16.
//

#include "TabuList.h"


TabuList::TabuList(int listSize, int _cadence) {
    tabuList = new Move*[listSize];
    size = listSize;
    cadence = _cadence;

    for (int i = 0; i < size; i++)
        tabuList[i] = new Move[listSize];

}

TabuList::~TabuList() {
        for(int i = 0; i < size; i++)
            if(tabuList[i] != nullptr)
                delete[] tabuList[i];
}

void TabuList::addMove(int moveBeg, int moveEnd) {
    tabuList[moveBeg][moveEnd].cadence = this->cadence;
    tabuList[moveBeg][moveEnd].usages++;

    tabuList[moveEnd][moveBeg].cadence = this->cadence;
    tabuList[moveEnd][moveBeg].usages++;
}

void TabuList::decrementCadence() {
    for(int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if(tabuList[i][j].cadence > 0)
                tabuList[i][j].cadence--;
        }
    }
}

void TabuList::removeFromList(int moveBeg, int moveEnd) {
    tabuList[moveBeg][moveEnd].cadence = 0;
}

bool TabuList::isOnTheList(int moveBeg, int moveEnd) {
    return (tabuList[moveBeg][moveEnd].cadence > 0) ? true : false;
}

void TabuList::clean() {
    for(int i = 0; i < size; i++)
        for(int k = 0; k < size; k++)
            removeFromList(i,k);
}

