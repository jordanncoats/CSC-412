//
//  Version 3 - distributor
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

#if 0
#pragma mark -
#pragma mark Function implementations
#endif

//--------------------------------------------------------------
//  Main function, expecting as arguments:
//        {Index} {Num of Items}
//  e.g.    ./distributor 2 6
//  This is meant be passed these arguments via Prog05(v3).
//--------------------------------------------------------------
int main(int argc, const char *argv[])
{
    //Stores the inputs as the index and num of files
    int index = strtol(argv[1], NULL, 10);
    int num = strtol(argv[2], NULL, 10);
    //A vector to hold fileNames.
    std::vector<std::string> fileName;
    //Reads the filenames and puses them back to the previous vector
    std::ifstream fN;
    fN.open("tempV3/d_" + std::to_string(index) + ".txt");
    int f = 0;
    //string to temp hold the line from d_*.txt file
    std::string file;
    while (getline(fN, file))
    {
        fileName.push_back(file);
    }
    //Creates a vector of dataItems, used throughout the function
    std::vector<dataItem> dataRead;
    //A loop to read the data from each filename it is given
    for (int i = 0; i < fileName.size(); i++)
    {
        std::ifstream reader;
        dataRead.push_back(dataItem());
        readData(&dataRead[i], fileName[i]);
    }
    // Goes through the read data to sort them into different p_*.txt files depending on what their
    // process_index is. These files are used by processor function to sort.
    for (int i = 0; i < num; i++)
    {
        std::string output;
        for (int j = 0; j < dataRead.size(); j++)
        {

            if (dataRead[j].process_index == i)
            {

                output.append(dataRead[j].file_path + "\n");
            }
        }
        std::ofstream out("tempV3/p_" + std::to_string(i) + ".txt", std::ios_base::app);
        out << output;
        out.close();
    }
    //exits when finished
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
    data->file_path = path;
    std::ifstream reader;
    reader.open(path);
    reader >> data->process_index;
    // reader >> data->line_number;
    // getline(reader, data->text);
}