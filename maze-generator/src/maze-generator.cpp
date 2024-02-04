/**
 * File: maze-generator.cpp
 * ------------------------
 * Presents an adaptation of Kruskal's algorithm to generate mazes.
 */

#include <iostream>
#include <algorithm>
#include <random>
#include <math.h>
#include <chrono>

#include "maze-graphics.h"
#include "maze-types.h"
#include "console.h"
#include "simpio.h"

using namespace std;


void unionSet(int row1, int col1, int row2, int col2, int** root, int& winnerNumber, int& loserNumber){
    if (root[row1][col1] < root[row2][col2]){
        winnerNumber = root[row1][col1];
        loserNumber = root[row2][col2];
    }
    else{
        winnerNumber = root[row2][col2];
        loserNumber = root[row1][col1];
    }
}

static int getMazeDimension(string prompt,
                            int minDimension = 7, int maxDimension = 50) {
    while (true) {
        int response = getInteger(prompt);
        if (response == 0) return response;
        if (response >= minDimension && response <= maxDimension) return response;
        cout << "Please enter a number between "
             << minDimension << " and "
             << maxDimension << ", inclusive." << endl;
    }
}

bool isNeighbor(cell one, cell two){
    if((abs(one.row - two.row) + abs(one.col - two.col)) == 1){
        return true;
    }
    else{
        return false;
    }
}

int main() {
    while (true) {
        int dimension = getMazeDimension("What should the dimension of your maze be [0 to exit]? ");
        if (dimension == 0) break;
        cout << "This is where I'd animate the construction of a maze of dimension " << dimension << "." << endl;

        MazeGeneratorView mgv;
        mgv.setDimension(dimension);

        int** root = new int*[dimension];
        for (int i = 0; i < dimension; ++i) {
            root[i] = new int[dimension];
        }
        for (int i = 0; i < dimension; i++){
            for (int j = 0; j < dimension; j++){
                root[i][j] = i * dimension + j;
            }
        }

        Set<cell> cellSet;
        Set<wall> wallSet;
        for (int i = 0; i < dimension; i++){
            for (int j = 0; j < dimension; j++){
                cell c;
                c.row = i;
                c.col = j;
                cellSet.add(c);
            }
        }

        for (cell c1 : cellSet){
            for (cell c2 : cellSet){
                if (isNeighbor(c1,c2)){
                    wall w;
                    w.one = c1;
                    w.two = c2;
                    if (c1 < c2){
                        wallSet.add(w);
                    }
                }
            }
        }

        mgv.drawBorder();
        mgv.addAllWalls(wallSet);
        mgv.repaint();

        Vector<wall> wallVector;
        for (wall w : wallSet){
            wallVector.add(w);
        }

        random_device rd;       // 使用 std::random_device 获取一个随机设备
        mt19937 rng(rd());      // 使用随机设备生成一个伪随机数引擎
        shuffle(wallVector.begin(), wallVector.end(), rng);
        
        Vector<cell> chamber;
        for (cell c : cellSet){
            chamber.add(c);
        }        
        for (wall w : wallVector){
            //if the wall seperates two chambers, then remove it
            if (root[w.one.row][w.one.col] != root[w.two.row][w.two.col]){
                mgv.removeWall(w);
                int winner,loser;
                unionSet(w.one.row, w.one.col, w.two.row, w.two.col, root, winner, loser);
                for (int i = 0; i < dimension; ++i){
                    for (int j = 0; j < dimension; ++j){
                        if (root[i][j] == loser){
                            root[i][j] = winner;
                        }
                    }
                }
            }
            mgv.repaint();
        }

        for (int i = 0; i < dimension; ++i) {
            delete[] root[i];
        }
        delete[] root;

        this_thread::sleep_for(chrono::milliseconds(10000));
    }
    return 0;
}
