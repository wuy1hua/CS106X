/**
 * File: life.cpp
 * --------------
 * Implements the Game of Life.
 */

#include <iostream>  // for cout
#include <fstream>
#include <thread>//for sleep
#include <chrono>
#include <cstdbool>
using namespace std;

#include "console.h" // required of all files that contain the main function
#include "simpio.h"  // for getLine
#include "gevent.h" // for mouse event detection
#include "strlib.h"

#include "life-constants.h"  // for kMaxAge
#include "life-graphics.h"   // for class LifeDisplay


/**
 * Function: welcome
 * -----------------
 * Introduces the user to the Game of Life and its rules.
 */
static void welcome() {
    cout << "Welcome to the game of Life, a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells live and die by the following rules:" << endl << endl;
    cout << "\tA cell with 1 or fewer neighbors dies of loneliness" << endl;
    cout << "\tLocations with 2 neighbors remain stable" << endl;
    cout << "\tLocations with 3 neighbors will spontaneously create life" << endl;
    cout << "\tLocations with 4 or more neighbors die of overcrowding" << endl << endl;
    cout << "In the animation, new cells are dark and fade to gray as they age." << endl << endl;
    getLine("Hit [enter] to continue....   ");
}

/**
 * Function: main
 * --------------
 * Provides the entry point of the entire program.
 */
int main() {
    LifeDisplay display;
    display.setTitle("Game of Life");
    welcome();
    cout << "You can start your colony with random cells or read from a prepared file." << endl;
    cout << "Enter name of colony file (or RETURN to seed randomly): " << endl;


    int Width = 0, Height = 0;
    string userInput;
    Grid<int> currentG(Height,Width);
    Grid<int> nextG(Height,Width);

    getline(cin,userInput);
    if(userInput.empty()){
        Width = randomInteger(40,60);
        Height = randomInteger(40,60);

        currentG.resize(Height,Width);
        nextG.resize(Height,Width);


        //initialize the cells randomly
        for (int row = 0; row < Height; row++){
            for (int col = 0; col < Width; col++){
                currentG[row][col] = randomBool();
                //if it exists, give it a random age
                if (currentG[row][col] == 1){
                    currentG[row][col] = randomInteger(1,kMaxAge);
                }
            }
        }
    }
    else{
        string filePath = "res/files/" + userInput;
        ifstream inputFile(filePath);
        if (!inputFile.is_open()) {
            cerr << "Failed to open the file!" << endl;
            return 1;
        }

        string line;

        bool Width_flag = false;
        bool Height_flag = false;

        while (getline(inputFile, line)) {
            if(line[0] != '#'){//skip the comments
                if(!Width_flag){
                    Width = stringToInteger(line);
                    Width_flag = true;
                }
                if(!Height_flag){
                    Height = stringToInteger(line);
                    Height_flag = true;
                }
                cout << line << endl;
            }
        }
        currentG.resize(Height,Width);
        nextG.resize(Height,Width);
        inputFile.close();

        inputFile.open(filePath);
        if (!inputFile.is_open()) {
            cerr << "Failed to open the file!" << endl;
            return 1;
        }
        int config_row = 0;
        while (getline(inputFile, line)) {
            if(line[0] != '#' && Width_flag && Height_flag && !line.empty()){
                for(int i = 0; i < line.length(); i++){
                    if(config_row < Height && i < Width){
                        if(line[i] == '-'){
                            currentG[config_row][i] = 0;
                        }
                        else if(line[i] == 'X'){
                            currentG[config_row][i] = 1;
                        }
                    }
                }
                config_row++;
            }
        }
        inputFile.close();
    }

    display.setDimensions(Height,Width);

    for (int generation =0; generation < kMaxAge; generation++){
        display.repaint();
        nextG = currentG;
        for (int row = 0; row < Height; row++){
            for (int col = 0; col < Width; col++){
                if(currentG[row][col] != 0){
                    nextG[row][col]++;
                }
                int neighbors = 0;

                if(row != 0 && row != Height -1 && col != 0 && col != Width -1){
                    for (int i = row - 1; i <= row + 1; i++){
                        if(i < 0){
                            continue;
                        }
                        if(i > Height - 1){
                            break;
                        }
                        for (int j = col - 1; j <= col + 1; j++){
                            if(j < 0){
                                continue;
                            }
                            if(j > Width - 1){
                                break;
                            }
                            if(i == row && j == col){
                                continue;//skip the centre
                            }
                            else if(i >= 0 && i < Height && j >= 0 && j < Width){
                                if(currentG[i][j] > 0){
                                    neighbors++;
                                }
                            }
                        }
                    }
                }

                //case die
                if(neighbors == 0 || neighbors == 1 || neighbors >= 4 ){
                    nextG[row][col] = 0;//die for loneliness or overcrowding
                }
                //case conceive
                else if(neighbors == 3){
                    if(!currentG[row][col]){
                        nextG[row][col] = 1;
                    }
                    else{}
                }
                //case stable
                else if(neighbors == 2){}
            display.drawCellAt(row,col,currentG[row][col]);
            }
        }
        //display.printBoard();
        currentG = nextG;
        this_thread::sleep_for(chrono::milliseconds(1000)); // 1000 milliseconds delay
    }
    return 0;
}
