//  CITS2002 Project 2 2020
//  Name:             Farshad Ghanbari
//  Student number:   21334883

/*
    dirManage.c:

    Created temporary directories

    Creates final output directory tree structure based on
    original strucure

    Removes temporary directories created and executes
    final cleanups
*/

#include "mergetars.h"

// Creates temporary directories based on TEMPLATE
void tempDirCreate(char* dest){
    strcpy(dest, TEMPLATE);
    if (mkdtemp(dest) == NULL){
        perror("mkdtemp() failed");
        exit(EXIT_FAILURE);
    }
}

// Creates output directory tree structure
void dirTreeCreate(void){
    // Stores remaining directories to be created
    int remaining = dirCount;

    while (remaining != 0){
        // Iterating through the directory structure
        for (int i = 0; i < dirCount; i++){
            // Created = 0 if directory has not been created yet
            if (directory[i].created == 0) {
                // make directory
                int status = mkdir(directory[i].dirPath, directory[i].dir_mode);

                // Status = 0 if directory was sucessfully created
                if (status == 0){
                    // Created = 1 since directory has been created
                    directory[i].created = 1;
                    remaining --;
                } else {
                    // e.g. temp/Dir1/Dir2/Dir3 might have lower index in structure than..
                    // e.g. temp/Dir1
                    // Therefore, Dir3 wont be created in this round
                    continue;
                }
            }
        }
    }
}

// Removes temporary directories created and free() memory
void dirRemove(void){
    
    // Processing fork()
    pid_t pid;
    for (int i = 0; i < tempDirectories->count; i++){
        switch (pid = fork()){
            case -1:
                perror("fork() failed");
                exit(EXIT_FAILURE);
                break;
            case 0:
                // Child process can execute rm commands  
                if ((execlp("rm", "rm", "-rf", tempDirectories[i].tempDirPath, NULL)) == -1){
                    perror("execlp() failed");
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                // Parent waits for child to finish
                wait(NULL);
                break;
        }

    }
    
    // Free previously allocated memories
    free(tarArchive);
    free(tempDirectories);
}