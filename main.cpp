#include <time.h>
#include "Grid.h"
#include "FileReader.h"
#include <iostream>
#include <chrono> //required to pause between generations
#include <thread> // to pause/sleep threads

using namespace std::this_thread;
using namespace std::chrono;

using namespace std;
bool keepRunning(Grid*& currentrentGen, Grid*& nextGen);

//global variable, used to count stabilized generations when checking
int stabilizeGens = 0;
//array used to hold the last three generation strings to check if it has been stabilized
string prevGenStrings[3];

int main()
{
  srand(time(NULL)); // makes sure that there is always different random numbers, using the current time
  int height = 0;
  int width = 0;
  float density = 0.0;
  int gameMode = 0;
  string userFile = "";
  Grid* currentGen;

  cout << "Welcome to the Game of Life! " << endl;
  cout << endl;
  cout << "Would you like to provide a map file of your grid or be given a random assignment?" << endl;
  cout << "Enter 1 to provide a file or 2 for a random assignment" << endl;
  int answerMapRandom;
  cin >> answerMapRandom;

  if(answerMapRandom == 1) // if the user provides a map file
  {
    cout << "Enter the name of the file you would like to use: ";
    cin >> userFile;
    cout << "What game mode would you like to play in?" << endl;
    cout << "Enter 1 for Classic mode, 2 for Doughnut mode, or 3 for Mirror Mode. ";
    cin >> gameMode;
    // checking if the gameMode that user entered is valid
    if(gameMode >= 1 & gameMode <= 3)
    {}
    else
    {
      cout << "You entered an invalid game mode. You cannot play. ";
      return 1;
    }
    try
    {
      currentGen = new Grid(userFile, gameMode);
    }
    catch(runtime_error e)
    {
      cout << "The file you entered does not exist or could not be opened. You cannot play. " << endl;
      return 1;
    }

    height = currentGen -> rows;
    width = currentGen -> columns;

  }
  else if (answerMapRandom == 2) // if the user wants a random assignment
  {
    cout << "Enter an integer for the number of rows: " << endl;
    cin >> height;
    // checking if user's entry for number of rows is valid
    if(height < 1)
    {
      cout << "You entered an invalid number of rows. You need to have at least 1 row. You cannot play." << endl;
      return 1;
    }
    cout << "Enter an integer for the number of columns: " << endl;
    cin >> width;
    // checking if user's entry for number of columns is valid
    if(width < 1)
    {
      cout << "You entered an invalid number of columns. You need to have at least 1 column. You cannot play." << endl;
      return 1;
    }
    cout << "Enter a decimal value (greater than 0 and less than or equal to 1) for the initial population density: " << endl;
    cin >> density;
    // checking if user's entry for density is valid
    if(density < 0.0)
    {
      cout << "You entered an invalid decimal for population density. You need have a decimal between 0 and 1. You cannot play." << endl;
      return 1;
    }
    else if(density > 1.0)
    {
      cout << "You entered an invalid decimal for population density. You need have a decimal between 0 and 1. You cannot play." << endl;
      return 1;
    }
    cout << "What game mode would you like to play in?" << endl;
    cout << "Enter 1 for Classic mode, 2 for Doughnut mode, or 3 for Mirror Mode. " << endl;
    cin >> gameMode;
    // checking if gameMode is valid
    if(gameMode >= 1 & gameMode <= 3)
    {}
    else
    {
      cout << "You entered an invalid game mode. You cannot play. " << endl;
      return 1;
    }
    // initializing currentGen with inputted dimensions
    currentGen = new Grid(height,width,density,gameMode);
  }
  else // if user did not enter a 1 or 2
  {
    cout << "You entered something other than a 1 or a 2. You cannot play. " << endl;
    return 1;
  }

  int outputResultsResponse = 0;
  cout << "Here are a few options to get your game results: " << endl;
  cout << "Enter 1 for a brief pause between your generations as they print out." << endl;
  cout << "Enter 2 for if you would like your generations to print out one by one, by pressing the 'Enter' key. " << endl;
  cout << "Enter 3 if you would like your game results to be printed in an output file. " << endl;
  cin >> outputResultsResponse;

  string outputFileName = "";

  if(outputResultsResponse == 3) // for option 3, asking user for an output file name
  {
    cout << "What would you like your output file to be named: " << endl;
    cin >> outputFileName;

  }

  // getting and printing the first generation grid for all three options
  if(outputResultsResponse == 1)
  {
    string currentGenOutputSt = currentGen -> getString();
    cout << "Generation 0: " << endl;
    cout << "Population Count: " << currentGen -> getPopulation(currentGenOutputSt) << endl;
    cout << currentGenOutputSt << endl;
    sleep_for(seconds(1)); // pausing for 1 second
  }
  else if(outputResultsResponse == 2)
  {
    string currentGenOutputSt = currentGen -> getString();
    cout << "Generation 0: " << endl;
    cout << "Population Count: " << currentGen -> getPopulation(currentGenOutputSt) << endl;
    cout << currentGenOutputSt << endl;
    cout << "Press 'ENTER' to continue to the next generation. " << endl;
    cin.get(); // taking in the enter key to go to the next generation

  }
  else if(outputResultsResponse == 3) //outputting first generation to file
  {
    string currentGenOutputSt = currentGen -> getString();
    string tempOut = "Generation 0: \n";
    FileReader::WriteToFile(outputFileName, tempOut);
    tempOut = "Population Count: " + to_string(currentGen -> getPopulation(currentGenOutputSt)) + "\n";
    FileReader::WriteToFile(outputFileName, tempOut);
    tempOut = currentGen -> getString() + "\n";
    FileReader::WriteToFile(outputFileName, tempOut);
  }
  else //checking if user did not enter 1, 2, or 3
  {
    cout << "You did not enter a 1, 2, or 3. You cannot play. " << endl;
    return 1;
  }


  int genCounter = 0; // used for counting each generation
  bool stillRunning = true;

  cin.get(); //clears the cin buffer

  // while loop that keeps running until keepRunning is true -- until grid has stabilized
  while (stillRunning)
  {

    // getting string for next generation, creating new grid object and printing each future generation grid
    string nextGenString = currentGen -> getNextGen();

    Grid* nextGen = new Grid(height, width, gameMode, nextGenString);
    // getting the string representing the grid
    string outputString = nextGen -> getString();
    cout << "Generation " << genCounter + 1 << endl;
    if(outputResultsResponse == 1)
    {
      cout << "Population count: " << nextGen -> getPopulation(outputString) << endl;
      cout << outputString << endl;

      sleep_for(seconds(1)); //pausing for one second between generations
    }
    else if(outputResultsResponse == 2)
    {
      cout << "Population count: " << nextGen -> getPopulation(outputString) << endl;
      cout << outputString << endl;

      cout << "Press 'ENTER' to continue to the next generation. " << endl;
      cin.get();
    }
    else if(outputResultsResponse == 3)
    {

      string tempOut = "Generation " + to_string((genCounter + 1)) + "\n";
      FileReader::WriteToFile(outputFileName, tempOut);
      tempOut = "Population Count: " + to_string((nextGen -> getPopulation(outputString))) + "\n";
      FileReader::WriteToFile(outputFileName, tempOut);
      FileReader::WriteToFile(outputFileName, outputString);
    }
    else
    {
      cout << "You did not enter a 1, 2, or 3. You cannot play. " << endl;
      return 1;
    }

    // places the last few generation strings into the prevGenStrings array, to check for stabilization
    prevGenStrings[genCounter % 3] = currentGen->getString();
    prevGenStrings[(genCounter + 1) % 3] = nextGen->getString();

    // checks if keepRunning is true
    // if true, while loop will iterate again
    // if false, break
    stillRunning = keepRunning(currentGen, nextGen);

    // using the deconstructor to delete the current grid object and reassigns current grid to the nextGen object
    currentGen -> ~Grid();
    currentGen = nextGen;

    genCounter++; // incrementing genCounter
  }
  //default
  return 0;

}

  // bool method that takes in grid object for the current and next generations
  // checks if stabilized:
  // if it has, return false
  // if not return true
  bool keepRunning(Grid*& currentGen, Grid*& nextGen)
  {
    // if there are no populated cells, return false
    if(nextGen->popCount == 0 )
      return false; //ends stillRunning loop

    // getting grids from current and next generation to check if they are both equal
    string currentGenStr = currentGen -> getString();
    string nextGenStr = nextGen -> getString();
    bool areGridsEqual = true;
    for(int i = 0; i < currentGenStr.length(); ++i)
    {
      if(currentGenStr[i] != nextGenStr[i])
      {
        areGridsEqual = false;
        break; // if even one character from the two grids are the same, there is no need to check again
      }
    }
    if (areGridsEqual == true)
    {
      return false; //ends stillRunning loop
    }

    //iterate through prevGenStrings to check for duplicate generations
    // if there is a duplicate, program has stabilized.
    for(int i = 0; i < 3; ++i)
    {
      for(int j = 0; j < 3; ++j)
      {
        //continue this iteration because we do not want to compare two of the exact same generations
        if(i == j)
          continue;

        // if it finds 2 generation strings that are equal, then we have a repeating simulation and program has stabilized
        if(prevGenStrings[i] == prevGenStrings[j])
          return false;

      }
    }

    //default
    return true;

    }
