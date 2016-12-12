//
// Created by piotrek on 01.12.16.
//

#ifndef TRAVELLINGSALESMANPROBLEMALGORITHMS_TABULIST_H
#define TRAVELLINGSALESMANPROBLEMALGORITHMS_TABULIST_H
#include <iostream>

class TabuQueue {
public:
    struct Move{
        int leftTown;
        int rightTown;
        int cadence;
        Move(){
            leftTown = -1;
            rightTown =-1;
            cadence = 0;
        }

        Move(int l, int r){
            leftTown = l;
            rightTown =r;
        }
    };

    TabuQueue(int _listLength);
    ~TabuQueue();

    void addMove(Move move);
    bool isOnTheList(Move move);
    int getCurrentLength(){return currentLength;}
    Move getMove(int i);
    void print();
private:
    int listLength;
    int currentLength;
    int listHead;
    Move* tabuList;
};


#endif //TRAVELLINGSALESMANPROBLEMALGORITHMS_TABULIST_H
