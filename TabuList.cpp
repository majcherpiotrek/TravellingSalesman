//
// Created by piotrek on 01.12.16.
//

#include "TabuList.h"


TabuList::TabuList(int listSize) {
    tabuList = new Move*[listSize];
    size = listSize;

    for (int i = 0; i < size; i++)
        tabuList[i] = new Move[listSize];

}

TabuList::~TabuList() {
        for(int i = 0; i < size; i++)
            if(tabuList[i] != nullptr)
                delete[] tabuList[i];
}

void TabuList::addMove(int moveBeg, int moveEnd) {
    tabuList[moveBeg][moveEnd].cadence = CADENCE_CONSTANT;
    tabuList[moveBeg][moveEnd].usages++;

    tabuList[moveEnd][moveBeg].cadence = CADENCE_CONSTANT;
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

