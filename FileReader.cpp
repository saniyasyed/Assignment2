#include "FileReader.h"

using namespace std;

//constructor
FileReader::FileReader(string userFile)
{
  ReadFile(userFile);
}

//void method used to read userFile
void FileReader::ReadFile(string userFile)
{
  //declaring ifstream obejct
  ifstream fileStream;
  //opening userFile through ifstream object
  fileStream.open(userFile);

  // checks if userFile fails to open and throws an error if true
  if(!fileStream)
  {
    throw runtime_error("Could not open " + userFile);
  }

  // counting number of lines
  string line;
  int lineCount = 0;
  //iterating through each line of the userFile
  while(getline(fileStream,line))
  {
    ++lineCount;
  }

  fileStream.clear();
  //resets fileStream to position 0, to be able to call getline() again
  fileStream.seekg(0);
  // making array to hold all the lines from the userFile
  string* fileLines = new string[lineCount];

  //place each line into the array
  int counter = 0;

  while(getline(fileStream,line))
  {
    fileLines[counter] = line;
    ++counter;
  }
  fileStream.clear();
  //resets fileStream to position 0, to be able to call getline() again
  fileStream.seekg(0);

  fileStream.close();

  //Error Checking -  making sure the first two lines are numbers representing the number of rows and columns
  //parsing the first two lines (rows and columns) into integers
  // lines after 1 and 2, followed by the grid

  //parsing first line (which represents the rows) into an integer
  this -> rows = stoi((string)fileLines[0]);
  //parsing second line (representing columns) into an integer
  this -> columns = stoi((string)fileLines[1]);
  // checking for file layout
  if(this -> rows != lineCount - 2 || isWidthCorrect(fileLines, lineCount, columns) == false)
  {
    throw runtime_error("This file does not have a correct layout. ");
  }

  int numCells = 0;
  int numPopCells = 0;
  // creating a fileGridString, adding each element to the string and counting the number of total cells and populated cells
  for(int i = 2; i < lineCount; ++i)
  {
    for(int j = 0; j < fileLines[i].length(); ++j)
    {
      numCells++;
      fileGridString += fileLines[i][j];
      if(fileLines[i][j] == 'X')
        ++numPopCells;
    }
  }

  // checking to make sure that the grid in the userFile only contains '-' or 'X'
  // if not, throws a runtime_error
  for (int i = 0; i < fileGridString.length(); ++i)
  {
    if(fileGridString[i] == 'X' || fileGridString[i] == '-')
    {}
    else
    {
      throw runtime_error("The file's grid should only contain the characters: '-' and 'X'." );
    }
  }
}


// bool method to check if the userFile's width is accurate with the grid in the file
// returns false if not accurate
bool FileReader::isWidthCorrect(string*& fileLines, int lineCount, int columns)
{
  for(int i = 2; i < lineCount; ++i)
  {
    if(fileLines[i].length() != columns)
      return false;
  }
  return true;
}
// void method to output to file if the user chooses to do so
void FileReader::WriteToFile(string userFile, string strToWrite)
{
  //declaring outputStream object and opening output file
  ofstream outfileStream;
  outfileStream.open(userFile, ofstream::app);
  // thrrowing runtime_error if output file cannot be opened 
  if(!outfileStream)
    throw runtime_error("Cannot open output file. ");
  outfileStream << strToWrite << endl;
  outfileStream.close();
}
