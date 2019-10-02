#include <iostream>
#include "Grid.h"
#include "FileReader.h"
#include <algorithm> // used for shuffling string representing the grid

using namespace std;

//constructor for declaring the first generation grid
Grid::Grid(int r,int c, float d, int g)
{
  rows = r;
  columns = c;
  density = d;
  gameMode = g;
  myGrid = new char*[rows];
  makeGrid();
  string gridString = stringMaker();
  gridInitialization(gridString);
}
//overloaded constructor - using for all next generations after the first generation
Grid::Grid(int r, int c, int g, string prevGen)
{
  rows = r;
  columns = c;
  gameMode = g;
  makeGrid();
  gridInitialization(prevGen);
  getPopulation(prevGen);

}
//overloaded constructor -- using when a user inputs a file to be read
Grid::Grid(string userFile, int gameMode)
{
  if(gameMode < 1 || gameMode > 3)
  {
    throw runtime_error("The game mode you entered was invalid. Please only enter one of the following: 1, 2 or 3. ");
  }
  this -> gameMode = gameMode;
  FileReader reader(userFile);
  rows = reader.rows;
  columns = reader.columns;
  makeGrid();
  gridInitialization(reader.fileGridString);
  getPopulation(reader.fileGridString);
}

//destructor
Grid::~Grid()
{
  for(int i = 0; i < rows; ++i){
    delete[] myGrid[i];
  }
  delete[] myGrid;
}

//void method to create a 2D array: a grid with dimensions(rows x columns) containing '-' at every index
void Grid::makeGrid()
{
  myGrid = new char*[rows];
  for(int i = 0; i < rows; ++i)
  {
    myGrid[i] = new char[columns];
    for(int j = 0; j < columns; ++j)
    {
      myGrid[i][j] = '-';
    }
  }
}

// string method to create a string called gridResult, adding each element of myGrid to gridResult
// returns gridResult
string Grid::getString()
{
  string gridResult = "";
  //iterating through myGrid
  for(int i = 0; i < rows; ++i)
  {
    for(int j = 0; j < columns; ++j)
    {
      //adding each element of myGrid to gridResult
      gridResult += myGrid[i][j];
    }
    // new line character to break line after every row is iterated through
    gridResult += "\n";

  }
  return gridResult;
}

//string method that creates a string, popString, that contains the population amount of "X" and a "-" for every remaining cell
// shuffles and returns pop string
string Grid::stringMaker()
{
  //calculating the population of the grid, using density and the total number of cells of the grid
  int totalCells = rows * columns;
  population = density * totalCells;
  population = (int)population;
  int remainingCells = totalCells - population;

  //adding "X" for every populated cell to the string
  for(int i = 0; i < population; ++i){
    popString += "X";
  }
  //adding "-" for all remaining cells that are not populated
  for(int i =0; i < remainingCells; ++i){
    popString += "-";
  }

  //shuffling popString to create a random grid with random positions for the "X"s and "-"s
  random_shuffle(popString.begin(),popString.end());

  return popString;
}


//void method to initialize grid
void Grid::gridInitialization(string popString)
{
  //iterating through empty grid (grid containing only '-')
  for(int i = 0; i < rows; ++i)
  {
      for(int j = 0; j < columns; ++j)
      {
        //setting each index of myGrid to the first character of popString
        //then, deleting the first character of popString, for every iteration, so that the first character of popString is the next element every iteration
        myGrid[i][j] = popString[0];
        popString.erase(0,1);
      }
  }

}

// bool method to check if a cell is a neighbor for the three different types of methods
bool Grid::isNeighbor(int x, int y, int centerX, int centerY)
{

  // Neighbor check for classic game mode
  if(gameMode == 1){
    //checking if a neighboring cell is outside the bounds of the grid
    // if it is, returns false
    if(x < 0)
      return false;
    if(y < 0)
      return false;
    if(x >= rows)
      return false;
    if(y >= columns)
      return false;
    //returns true for a populated cell, false for an empty cell
    if(myGrid[x][y] == '-')
      return false;
    if(myGrid[x][y] == 'X')
      return true;
  }


  // Neighbor check for doughnut game mode
  else if(gameMode == 2)
  {
      //uses a method to check if the cell is on the border of the grid
      //if true, reassigns x and y values to adapt to the doughnut game mode rules
      if(isOnBorder(centerX, centerY))
      {

        // if the row value is out of the grid bounds, either above or below the grid :
        //program will wrap around to the grid's opposite side to reassign the x value, representing the row
        if(x < 0)
          x = rows - 1;
        else if(x >= rows)
          x = 0;

        // if the column value is out of the grid bounds, either to the left or right side of the grid :
        // program will wrap around to the grid's opposite side to reassign the y value, representing the column
        if(y < 0)
          y = columns - 1;
        else if(y >= columns)
          y = 0;
      }
      //returns true for a populated cell, false for an empty cell
      if(myGrid[x][y] == 'X')
        return true;
      else
        return false;

  }
  // Neighbor check for mirror game mode
  else if(gameMode == 3)
  {
    //uses a method to check if the cell is on the border of the grid
    //if true, reassigns x and y values to adapt to the mirror game mode rules
    if(isOnBorder(centerX, centerY))
    {
      // if the row value is out of the grid bounds, either above or below the grid :
      //program will reassign x value (representing the row) to the value right above it (mirroring the cell next to it)
      if(x < 0)
        x = 0;
      else if(x >= rows)
        x = rows - 1;
      // if the column value is out of the grid bounds, either to the left or right side of the grid :
      // program will reassign y value (representing the column) to the value right next to it (mirroring the cell next to it)
      if(y < 0)
        y = 0;
      else if(y >= columns)
        y = columns - 1;
    }
    //returns true for a populated cell, false for an empty cell
    if(myGrid[x][y] == 'X')
      return true;
    else
      return false;

  }

}

// int method used to count all the neigbors, using the isNeighbor() method
// checks all different combinations, to check if all neigboring cells are populated
// increments totalNeighbors for every populated cell
int Grid::neighborCount(int x, int y)
{
  //checking each combination of cells to check if it is a neighbor, using isNeighbors()
  int totalNeighbors = 0;
  if(isNeighbor(x+1,y,x,y))
    totalNeighbors++;
  if(isNeighbor(x-1,y,x,y))
    totalNeighbors++;
  if(isNeighbor(x,y+1,x,y))
    totalNeighbors++;
  if(isNeighbor(x,y-1,x,y))
    totalNeighbors++;
  if(isNeighbor(x+1,y+1,x,y))
    totalNeighbors++;
  if(isNeighbor(x-1,y-1,x,y))
    totalNeighbors++;
  if(isNeighbor(x-1,y+1,x,y))
    totalNeighbors++;
  if(isNeighbor(x+1,y-1,x,y))
    totalNeighbors++;

  return totalNeighbors;

}

// string method to get the next generation grid in the form of a string, gridStr
// returns gridStr
string Grid::getNextGen()
{
  string gridStr = "";

  // iterating through myGrid and checking each rule for the Game of Life
  // determines if each cell becomes and 'X' or '-' in the next generation
  for(int i = 0; i < rows; ++i)
  {
    for(int j = 0; j < columns; ++j)
    {
      //checking all rules for the Game of Life if the original cell is populated
      // adds character for next generation to the gridStr based on current grid
      if(myGrid[i][j] == 'X')
      {
        if(neighborCount(i,j) == 2)
        {
          gridStr += "X";
        }
        else if(neighborCount(i,j) == 3)
        {
          gridStr += "X";
        }
        else
        {
          gridStr += "-";
        }
      }
      //checking all rules for the Game of Life if the original cell is empty
      // adds character for the next generation to the gridStr based on current grid
      else if(myGrid[i][j] == '-')
      {
        if(neighborCount(i,j) == 3)
        {
          gridStr += "X";
        }
        else
        {
          gridStr += "-";
        }
      }
    }
  }
  return gridStr;
}

// void method the get the population for each grid, taking in a string of all the grid elements
int Grid::getPopulation(string inputStr)
{
    popCount = 0;
    // iterating through the input string
    for(int i = 0; i < inputStr.length(); ++i)
    {
      // incrementing popCount for every 'X' in the string
      if(inputStr[i] == 'X')
        popCount++;
    }
    return popCount;
}


// bool method used to check if a cell is on the grid's border
// returns true if it is on the border, false if it is not
bool Grid::isOnBorder(int x, int y)
{
  if(x == 0)
    return true;
  else if(x == (rows - 1))
    return true;


  if(y == 0)
    return true;
  else if(y == (columns - 1))
    return true;

  // default 
  return false;
}
