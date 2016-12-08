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
    TabuList(int listSize, int _cadence);
    ~TabuList();

    void addMove(int moveBeg, int moveEnd);
    void decrementCadence();
    void removeFromList(int moveBeg, int moveEnd);
    void clean();
    bool isOnTheList(int moveBeg, int moveEnd);

private:
    int cadence;
    Move** tabuList;
    int size;
};


#endif //TRAVELLINGSALESMANPROBLEMALGORITHMS_TABULIST_H
