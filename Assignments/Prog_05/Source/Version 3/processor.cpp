//
//  Version 3 - processor
//  Prog 05 Solution
//
//  Created by Jordan Coats
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include "datastruct.h"

#if 0
#pragma mark -
#pragma mark Function prototypes
#endif

/** A function to take the contents of the text file and put them into a struct type dataItem
  *  @param data the struct to be worked with.
  *  @param path the path of the to pull contents from.
  *  @return  a dataItem with contents of a file to be added to a vector.
  */
void readData(dataItem *data, const std::string &path);
/** Used with sort to organize line numbers.
  *  @param a a value to compare.
  *  @param b a value to compare.
  *  @return  a true or false if they're in order or not.
  */
bool compareLine(const dataItem &a, const dataItem &b);

#if 0
#pragma mark -
#pragma mark Function implementations
#endif

//--------------------------------------------------------------
//  Main function, expecting as arguments:
//        {Number of Processes}
//  e.g.    ./processor 2
//  This is meant be passed these arguments via Prog05(v3).
//--------------------------------------------------------------
int main(int argc, const char *argv[])
{
    //Stores the input as the index.
    int index = strtol(argv[1], NULL, 10);
    //An output string to store the completed ordered code eventually
    std::string output;
    //A string to hold the fileNames
    std::string fileName;
    //Open the ifstream to pull data into the vector<dataItem> vector.
    std::ifstream reader;
    std::vector<dataItem> dataRead;
    //This reads the file created by distributor.

    reader.open("tempV3/p_" + std::to_string(index) + ".txt");
    int i = 0;
    while (reader >> fileName)
    {
        dataRead.push_back(dataItem());
        readData(&dataRead[i], fileName);
        i++;
    }
    //Once it has everything it needs it can then sort them by line number.
    std::sort(dataRead.begin(), dataRead.end(), compareLine);
    //Now that it is in order it reads and appends each text string to the output.
    for (int i = 0; i < dataRead.size(); i++)
    {
        output.append(dataRead[i].text + "\n");
    }
    //Because each processor only works with one index it only needs to create the one output file.
    std::ofstream out("tempV3/o_" + std::to_string(index) + ".txt", std::ios_base::app);
    out << output;
    reader.close();
    out.close();
    exit(0);
}
// ---------------------------------------------------------------------
//  Function : readData
//  Description :
//
//  This function reads a text file to a dataItem type.
//  As it doesn't need them I commended out a few lines.
//
//----------------------------------------------------------------------
void readData(dataItem *data, const std::string &path)
{
    //data->file_path = path;
    std::ifstream reader;
    reader.open(path);
    reader >> data->process_index;
    reader >> data->line_number;
    reader >> std::ws;
    getline(reader, data->text);
}
// ---------------------------------------------------------------------
//  Function : compareLine
//  Description :
//
//  A utility function to compare the line numbers of the type dataItem.
//
//----------------------------------------------------------------------
bool compareLine(const dataItem &a, const dataItem &b)
{
    return a.line_number < b.line_number;
}