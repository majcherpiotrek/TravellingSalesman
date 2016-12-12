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
    TabuMatrix(int s, int tt ){
        size = s;
        tabuTenure = tt;
        tabuMatrix = new int*[size];
        for (int i = 0; i < size; ++i) {
            tabuMatrix[i] = new int [tabuMatrix];
            for (int j = 0; j < size ; ++j)
                tabuMatrix[i][j] = 0; //tabuTenure = 0
        }
    }

    void addMove(int a, int b){
        tabuMatrix[a][b] += tabuTenure;
    }

    bool isTabu(int a, int b){
        if(tabuMatrix[a][b] > 0)
            return true;
        else
            return false;
    }

    void decrementTenure(){
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j)
                if(isTabu(i,j))
                    tabuMatrix[i][j]--;
        }
    }
};


#endif //TRAVELLINGSALESMAN_TABUMATRIX_H
