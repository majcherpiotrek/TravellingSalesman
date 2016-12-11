//
// Created by piotrek on 01.12.16.
//

#ifndef TRAVELLINGSALESMANPROBLEMALGORITHMS_TABULIST_H
#define TRAVELLINGSALESMANPROBLEMALGORITHMS_TABULIST_H

class TabuList {
public:
    struct Move{
        int leftTown;
        int rightTown;

        Move(){
            leftTown = -1;
            rightTown =-1;
        }

        Move(int l, int r){
            leftTown = l;
            rightTown =r;
        }
    };

    TabuList(int _listLength);
    ~TabuList();

    void addMove(Move move);
    bool isOnTheList(Move move);

private:
    int listLength;
    int currentLength;
    int listHead;
    Move* tabuList;
};


#endif //TRAVELLINGSALESMANPROBLEMALGORITHMS_TABULIST_H
