//
// Created by piotrek on 12.12.16.
//

#ifndef TRAVELLINGSALESMAN_TABUMATRIX_H
#define TRAVELLINGSALESMAN_TABUMATRIX_H

class TabuMatrix {
private:
    int** tabuMatrix;
    int size;
    int tabuTenure;

public:
    TabuMatrix(int s, int tt );


    void addMove(int a, int b){
        tabuMatrix[a][b] += tabuTenure;
    }

    bool isTabu(int a, int b);

    void decrementTenure(){
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j)
                if(isTabu(i,j))
                    tabuMatrix[i][j]--;
        }
    }
};


#endif //TRAVELLINGSALESMAN_TABUMATRIX_H
