#include <iostream>
#include <fstream>

using namespace std;

class FileReader
{
public:
  //constructor to read file
  FileReader(string userFile);

  //global functions
  void ReadFile(string userFile);
  bool isWidthCorrect(string*& fileLines, int lineCount, int columns);

  static void WriteToFile(string userFile, string strToWrite);

  //global variables
  int rows;
  int columns;
  string fileGridString;
};
