//  CITS2002 Project 2 2020
//  Name:             Farshad Ghanbari
//  Student number:   21334883

/*  mergetars is a utility program that merges the 
    contents of multiple tar archive files into a 
    single tar archive file.
*/

#include "mergetars.h"

int main(int argc, char *argv[]){
    if (argc < 3) {
        perror("too few arguments provided\n");
        exit(EXIT_FAILURE);
    }
    
    // Initialising Global Variables
    initialiseGlobals(argc - 2);

    // Creating Final output directory
    // Index 0 holds final output directory
    tempDirCreate(tempDirectories[0].tempDirPath);
    tempDirectories->count++;

    // Expands each tar file provided in the argument
    tarExpand(argv + 1);
    // Applies merging criteria
    // Selected files to be exported into output directory
    selectFiles();

    // Creates output directory structure based on original
    dirTreeCreate();

    // Copy selected files to directory structure created above
    copyFiles();

    // Create final tar file
    tarCreate(argv[argc - 1]);

    // Clean up temp dirs created and free memory
    dirRemove();

    exit(EXIT_SUCCESS);
}