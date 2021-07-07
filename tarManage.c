//  CITS2002 Project 2 2020
//  Name:             Farshad Ghanbari
//  Student number:   21334883

/*
    tarManage.c:

    Extracts tar files and stores archive stats

    Creates final tar file
*/

#include "mergetars.h"

// Expands tar Paths provided in the argument into a new temp directory
void tarExpand(char* tarPaths[]){
    
    // stat structure to store tarfile's details into tarArchive struct
    struct stat statbuf;

    // forking the parent process to execute tar extraction on each file
    for (int i = 0; i < numTarFiles; i++){
        // Creating a new unique directory
        int n = tempDirectories->count;
        tempDirCreate(tempDirectories[n].tempDirPath);
        char* tempDir = tempDirectories[n].tempDirPath;
        tempDirectories->count++;
        
        // Processing fork()
        pid_t pid;
        switch (pid = fork()){
            case -1:
                perror("fork() failed");
                exit(EXIT_FAILURE);
                break;
            case 0:
                // Child process can execute tar commands
                if ((execlp("tar", "tar", "-xpf", tarPaths[i], "-C", tempDir, NULL)) == -1){
                    perror("execlp() failed");
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                // Parent records file details into tarArchive struct
                if (stat(tarPaths[i], &statbuf) == -1) {
                    perror("stat() failed");
                    exit(EXIT_FAILURE);
                }
                // setting last modification time of archive
                tarArchive[i].modTime = statbuf.st_mtime;
                // initialising number of files in this archive
                tarArchive[i].count = 0;

                // Parent waits for child to finish tar extraction 
                wait(NULL);
                // Parent looks through the extracted Paths
                // Passing length for string manipulation
                int dirNameLength = strlen(tempDir);
                listFiles (tempDir, i, dirNameLength);
                break;
        }
    }
}

// Creates final tar file
void tarCreate(char* outputPath){

    char* source = tempDirectories[0].tempDirPath;

    // Getting type of requested output tar
    char* tarType = strchr(outputPath, '.');
    // Processing fork()
    pid_t pid;
    switch (pid = fork()){
            case -1:
                perror("fork() failed");
                exit(EXIT_FAILURE);
                break;
            case 0:
            // Child process can execute tar commands
                if (strcmp(tarType, ".tar") == 0){
                    if ((execlp("tar", "tar", "-cpf", outputPath, "-C", source, ".",  NULL)) == -1){
                        perror("execlp() failed");
                        exit(EXIT_FAILURE);
                    }
                } else if ((strcmp(tarType, ".tar.gz") == 0) || strcmp(tarType, ".tgz") == 0){
                    if ((execlp("tar", "tar", "-cpzf", outputPath, "-C", source, ".",  NULL)) == -1){
                        perror("execlp() failed");
                        exit(EXIT_FAILURE);
                    }                   
                }
                break;
            default:
                // Parent waits for child to finish
                wait(NULL);
                break;
    }
}