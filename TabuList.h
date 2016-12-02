//
// Created by piotrek on 01.12.16.
//

#ifndef TRAVELLINGSALESMANPROBLEMALGORITHMS_TABULIST_H
#define TRAVELLINGSALESMANPROBLEMALGORITHMS_TABULIST_H

class TabuList {
public:
    struct Move{
        int cadence=0;
        int usages=0;
    };
    TabuList(int listSize);
    ~TabuList();

    void addMove(int moveBeg, int moveEnd);
    void decrementCadence();
    void removeFromList(int moveBeg, int moveEnd);
    bool isOnTheList(int moveBeg, int moveEnd);

private:
    static const int CADENCE_CONSTANT = 50;
    Move** tabuList;
    int size;
};


#endif //TRAVELLINGSALESMANPROBLEMALGORITHMS_TABULIST_H
