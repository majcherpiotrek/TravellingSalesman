//
// Created by piotrek on 12.12.16.
//

#include "TabuMatrix.h"

TabuMatrix::TabuMatrix(int s, int tt) {
    size = s;
    tabuTenure = tt;
    tabuMatrix = new int*[size];
    for (int i = 0; i < size; ++i) {
        tabuMatrix[i] = new int [size];
        for (int j = 0; j < size ; ++j)
            tabuMatrix[i][j] = 0; //tabuTenure = 0
    }
}

bool TabuMatrix::isTabu(int a, int b) {
    if(tabuMatrix[a][b] > 0)
        return true;
    return false;
}

void TabuMatrix::resetTabu() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j)
            tabuMatrix[i][j] = 0;
    }
}
