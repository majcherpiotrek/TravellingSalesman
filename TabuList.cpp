//
// Created by piotrek on 01.12.16.
//

#include "TabuList.h"


TabuList::TabuList(int _listLength) {
    listLength = _listLength;
    tabuList = new Move[listLength];
    for (int i = 0; i < listLength; ++i)
        tabuList[i] = *(new Move());

    listHead = 0;
    currentLength = 0;
}

TabuList::~TabuList() {
    if(tabuList != nullptr)
        delete[] tabuList;
}

void TabuList::addMove(Move move) {
    if(!isOnTheList(move)){
        //Jeśli lista nie jest jeszcze zapełniona
        if(tabuList[listHead].leftTown == -1)
            currentLength++; //to zwiększamy zapełnienie listy

        tabuList[listHead++] = move;
        //Jeśli jesteśmy na ostatnim wolnym miejscu na liście
        if(listHead == listLength)
            listHead = 0; //to wracamy na początek listy
    }
}

bool TabuList::isOnTheList(Move move) {

    for (int i = 0; i < currentLength; ++i)
        if(tabuList[i].leftTown == move.leftTown && tabuList[i].rightTown == move.rightTown)
            return true;

    return false;
}

TabuList::Move TabuList::getMove(int i) {

    if (i < 0 || currentLength <= i) return Move();
    int idx = listHead - 1 - i;
    if (idx < 0) idx = listLength + idx;
    return tabuList[idx];

}

void TabuList::print() {
    for(int i = 0; i< listLength; i++){
        std::cout.flush();
        std::cout << "(" << tabuList[i].leftTown << "," << tabuList[i].rightTown << ")";
    }
}
