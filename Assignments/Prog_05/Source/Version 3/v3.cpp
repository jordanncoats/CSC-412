//
//  Version 3
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
#include <sys/types.h>
#include <sys/stat.h>
#include <ftw.h>
#include "datastruct.h"

#if 0
#pragma mark -
#pragma mark Function prototypes
#endif

/** Function created by Prof Herve, slightly modified by myself. It reads every file in the directory
  *  creates a list of those files and stores them in a vector.
  *  @param  dataRootPath the path from argv[2].
  *  @param fileName reference to the vector it will work with.
  *  @return  a modifed vector with the filepaths.
  */
void filenamePath(const char *dataRootPath, std::vector<std::string> &fileName);
/** This function takes the filename vector and splits it in mostly even lots.
  *  creates a list of those files and stores them in a vector.
  *  @param fileName reference to the vector it will work with.
  *  @param k the number of processes that will eventually be created.
  *  @return  a vector of vector<strings> that holds each lot of sorted filenames.
  */
std::vector<std::vector<std::string>> SplitVector(const std::vector<std::string> &vec, const int k);
/** Simple function to find the min of two values, implemented as to not have to use the whole math library.
  *  @param a a value to compare
  *  @param b a value to compare
  *  @return  the min value.
  */
int min(int a, int b);
/** A function to take the contents of the text file and put them into a struct type dataItem
  *  @param data the struct to be worked with.
  *  @param path the path of the to pull contents from.
  *  @return  a dataItem with contents of a file to be added to a vector.
  */
void readData(dataItem *data, const std::string &path);
/** This function takes one of the split list of filenames and orders it by the process index.
  * to write to a text file of that process index by creating a child process that execs.
  *  @param k the number of processes.
  *  @param n the index it is working with itself.
  *  @param fileName a vector of filenames to work with.
  *  @param pid A vector of PIDs for the waitpid till the child is finished.
  *  @return  a text file of each process index with the correct filespaths inside.
  */
void childDistributor(const int k, const int n, std::vector<pid_t> &pid);
/** This function takes the distributor's completed text file and sorts it by the line number to create
 *  a text file of ordered code by creating a child process that execs.
  *  @param k the index it works with.
  *  @param pid A vector of PIDs for the waitpid till the child is finished.
  *  @return  a text file with a correctly sorted by line number chunk of code.
  */
void childProcessor(const int k, std::vector<pid_t> &pid);
/** Used with sort to organize line numbers.
  *  @param a a value to compare.
  *  @param b a value to compare.
  *  @return  a true or false if they're in order or not.
  */
bool compareLine(const dataItem &a, const dataItem &b);
/** This function was found on StackOverflow, it isn't /needed/ for the assignment but helps keep the 
 *  the directory clean by removing temp files at the end.
 *  I believe it originates from "The Linux Programming Interface."
  *  @param pathname the path to be checked.
  *  @param sbuf a stat struct.
  *  @param type a type value.
  *  @param ftwb a FTW struct.
  *  @return  the min value.
  */
static int rmFiles(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb);

#if 0
#pragma mark -
#pragma mark Function implementations
#endif

int main(int argc, const char *argv[])
{
    //Checks argv[2] for the data path, will add a "/" if its nonexistant
    const char *dataRootPath;
    std::string path = argv[2];
    if (path[path.size() - 1] != '/')
    {
        path.append("/");
        dataRootPath = (char *)path.c_str();
    }
    else
    {
        dataRootPath = argv[2];
    }

    //Checks argv[1] for the number of processes counted by the script
    const long procNum = strtol(argv[1], NULL, 10);

    //Checks argv[3] for the final output path, since it is a .c if it isn't already given it will add the extentsion.
    std::string outPath = argv[3];
    if (outPath[outPath.size() - 1] != 'c' && outPath[outPath.size() - 2] != '.')
    {
        outPath.append(".c");
    }

    //Creates a vector to store the folder's filenames, then sorts them into mostly equal lots.
    std::vector<std::string> fileName;
    filenamePath(dataRootPath, fileName);

    struct stat info;
    //Creates a folder called "tempV*" to store all the temp text files created.
    if (stat("tempV3", &info) != 0)
        system("mkdir tempV3");
    else if (info.st_mode & S_IFDIR)
        printf("Using %s as a temporary directory. Will be deleted...\n", "tempV3");

    std::vector<std::vector<std::string>> fileName_Split = SplitVector(fileName, procNum);
    // A loop new for V3 that creates a text file for the exec distributor to read from since
    // it would be difficult to pass everything with argvs.
    for (int i = 0; i < procNum; i++)
    {
        std::ofstream out("tempV3/d_" + std::to_string(i) + ".txt", std::ios_base::app);
        for (int j = 0; j < fileName_Split[i].size(); j++)
        {
            out << dataRootPath + fileName_Split[i][j] + "\n";
        }
        out.close();
    }
    //A loop set to create distribute children for each split list
    std::vector<pid_t> pidD;
    for (int i = 0; i < procNum; i++)
    {
        childDistributor(i, procNum, pidD);
    }
    //To make sure each child has finished before the next function is called
    for (int i = 0; i < procNum; i++)
    {
        int returnStatus;
        waitpid(pidD[i], &returnStatus, 0);

        if (returnStatus == 1)
        {
            std::cout << "The child process terminated with an error!" << std::endl;
        }
    }
    //A second loop set to take the now organized distrubted lists and sort them by line number via child processes
    std::vector<pid_t> pidP;
    for (int i = 0; i < procNum; i++)
    {
        childProcessor(i, pidP);
    }
    //To make sure each child has finished before combining the output text files
    for (int i = 0; i < procNum; i++)
    {
        int returnStatus;
        waitpid(pidP[i], &returnStatus, 0);

        if (returnStatus == 1)
        {
            std::cout << "The child process terminated with an error!" << std::endl;
        }
    }
    //Create the output file and loops to organize each section of code into the correct order
    std::ofstream combined_file(outPath);

    for (int i = 0; i < procNum; i++)
    {

        std::ifstream file("tempV3/o_" + std::to_string(i) + ".txt");

        combined_file << file.rdbuf();

        file.close();
    }
    combined_file.close();
    //Deletes the temp folder
    nftw("tempV3", rmFiles, 10, FTW_DEPTH | FTW_MOUNT | FTW_PHYS);
    return 0;
}
// ---------------------------------------------------------------------
//  Function : fileNamePath
//  Description :
//
//  Prof Herve's code.
//
//----------------------------------------------------------------------
void filenamePath(const char *dataRootPath, std::vector<std::string> &fileName)
{
    DIR *directory = opendir(dataRootPath);
    if (directory == NULL)
    {
        std::cout << "data folder " << dataRootPath << " not found" << std::endl;
        exit(1);
    }

    struct dirent *entry;

    while ((entry = readdir(directory)) != NULL)
    {
        const char *name = entry->d_name;
        if ((name[0] != '.') && (entry->d_type == DT_REG))
        {
            fileName.push_back(std::string(entry->d_name));
        }
    }
    closedir(directory);
}

// ---------------------------------------------------------------------
//  Function : readData
//  Description :
//
//  This function reads a text file to a dataItem type.
//
//----------------------------------------------------------------------
void readData(dataItem *data, const std::string &path)
{
    //Stores the filename it was passed.
    data->file_path = path;
    //Opens up a ifstream to read from the file.
    std::ifstream reader;
    reader.open(path);
    //stores the first value as the process_index.
    reader >> data->process_index;
    //The second value as line_number.
    reader >> data->line_number;
    //Reads the rest of the file in text.
    getline(reader, data->text);
}
// ---------------------------------------------------------------------
//  Function : childDistributor
//  Description :
//
//  This function distributes a collection of text files into a list of which
//  process should handle the each then creates a child to complete that task
//
//----------------------------------------------------------------------
void childDistributor(const int k, const int n, std::vector<pid_t> &pid)
{
    //forks into the child
    pid_t p = fork();
    if (p == 0)
    {
        //Herve helped to convert to char* for the args.
        std::string k_s = std::to_string(k);
        std::string n_s = std::to_string(n);
        //launches the exec with the distributor program
        char *args[] = {"./distributor", (char *)(k_s.c_str()), (char *)(n_s.c_str()), NULL};
        int status = execvp(args[0], args);
        //if it doesn't launch will post the status code as an error
        printf("STATUS CODE=%d\n", status);
    }
    else
    {
        //pushes the pid into the vector for the wait list
        pid.push_back(p);
    }
}
// ---------------------------------------------------------------------
//  Function : childProcessor
//  Description :
//
//  Is given an index then opens the distributor's text file of that index
//  to sort the contents by line number by child process.
//
//----------------------------------------------------------------------
void childProcessor(const int k, std::vector<pid_t> &pid)
{
    //forks into the child.
    pid_t p = fork();
    if (p == 0)
    {
        //Again Herve helped to convert to char* for the args.
        std::string k_s = std::to_string(k);
        char *args[] = {"./processor", (char *)(k_s.c_str()), NULL};
        int status = execvp(args[0], args);
        //if it doesn't launch will post the status code as an error
        printf("STATUS CODE=%d\n", status);
    }
    else
    {
        //pushes the pid into the vector for the wait list
        pid.push_back(p);
    }
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

// ---------------------------------------------------------------------
//  Function : rmFiles
//  Description :
//
//  A utility function to remove a directory. Taken from Stackoverflow.
//
//----------------------------------------------------------------------
static int rmFiles(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{
    if (remove(pathname) < 0)
    {
        perror("ERROR: remove");
        return -1;
    }
    return 0;
}

// ---------------------------------------------------------------------
//  Function : min
//  Description :
//
//  A utility function to return the min of two values.
//
//----------------------------------------------------------------------
int min(int a, int b)
{
    return (a <= b) ? a : min(b, a);
}

// ---------------------------------------------------------------------
//  Function : SplitVector
//  Description :
//
//  This function takes the list of filenames and how many processors will be
//  created to divide the list into mostly equal parts which are stored in a
//  vector to be used by the other processes.
//
//----------------------------------------------------------------------
std::vector<std::vector<std::string>> SplitVector(const std::vector<std::string> &fileName, int k)
{
    // A vector that has the first value for the distributor that will work with that portion then the
    // inner vector holds the strings of the filename.
    std::vector<std::vector<std::string>> outVec;

    //The mostly even division of each list by the number of files.
    int length = fileName.size() / k;
    int remain = fileName.size() % k;

    int begin = 0;
    int end = 0;
    //Loop to push back the values into the position that it needs to be.
    for (int i = 0; i < min(k, fileName.size()); ++i)
    {

        if (remain > 0)
        {
            end += (length + !!(remain--));
        }
        else
        {
            end += length;
        }

        outVec.push_back(std::vector<std::string>(fileName.begin() + begin, fileName.begin() + end));

        begin = end;
    }
    return outVec;
}