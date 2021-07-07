    //  CITS2002 Project 2 2020
//  Name:             Farshad Ghanbari
//  Student number:   21334883

#include "mergetars.h"

// Number of tar files entered in argument
int numTarFiles;

// Total number of directories read
int dirCount;

// Stores details of temporary directories
// Index 0 holds final output directory
tempDirs* tempDirectories;

// stores details of original directory tree structure
dir directory[dirMAX];

// Stores archive details and its file details
archive* tarArchive;

void initialiseGlobals (int numTars){ 
    numTarFiles = numTars;

    dirCount = 0;

    tempDirectories = (tempDirs*) malloc ((numTarFiles + 1) * sizeof(tempDirs));
    if (tempDirectories == NULL){
        perror("malloc() for tempDirectories failed");
        exit(EXIT_FAILURE);
    }

    tarArchive = (archive*) malloc (numTarFiles * sizeof(archive));
    if (tarArchive == NULL){
        perror("malloc() for tarArchive failed");
        exit(EXIT_FAILURE);
    }
}
