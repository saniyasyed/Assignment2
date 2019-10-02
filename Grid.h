#include <iostream>
using namespace std;

class Grid
{
  public:
    //constructor to construct grid object
    Grid(int rows, int columns, float density, int gameMode);
    //overloaded constructor - for next generations
    Grid(int rows, int columns, int gameMode, string prevGen);
    //overloaded constructor - used for reading userFiles
    Grid(string userFile, int gameMode);
    //destructor to delete grid object - used for deleting each generation after going to the next 
    ~ Grid();

    //global methods
    void makeGrid();
    string getString();
    string stringMaker();
    void gridInitialization(string popString);
    int getPopulation(string inputString);
    int neighborCount(int centerX, int centerY);
    string getNextGen();
    bool isNeighbor(int x, int y, int centerX, int centerY);
    bool isOnBorder(int centerX, int centerY);

    //global variables
    int rows;
    int columns;
    int population;
    float density;
    string popString;
    int gameMode;
    int popCount;

    char** myGrid;

};
