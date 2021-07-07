//  CITS2002 Project 2 2020
//  Name:             Farshad Ghanbari
//  Student number:   21334883

/*
    fileManage.c:

    Open and reads directory, storing its subdirectory and file
    details in their respective structures.

    Applies merging criteria to select files for final tar archive.

    Copies selected files to new location
*/

#include "mergetars.h"
// Stored file details in files struct within archive struct
void addFileStats(char* dirName, const char* d_name, int archiveNum, int dirNameLength){
    struct stat statbuff;
    int fileNum = tarArchive[archiveNum].count;
    
    // Getting full path name of the file
    char fullFilePath[PATH_MAX];
    strcpy(fullFilePath, dirName);
    strcat(fullFilePath, "/");
    strcat(fullFilePath, d_name);
    strcpy(tarArchive[archiveNum].file[fileNum].fullPath, fullFilePath);

    // Running stat on the file
    if (stat(fullFilePath, &statbuff) == -1){
        perror("stat() failed");
        exit(EXIT_FAILURE);
    }

    // Stores size and last modification time of the file
    tarArchive[archiveNum].file[fileNum].size   = statbuff.st_size;
    tarArchive[archiveNum].file[fileNum].mtime  = statbuff.st_mtime;
    tarArchive[archiveNum].file[fileNum].f_mode = statbuff.st_mode;
    
    // initialising the 'selected' field: More info in strcture
    tarArchive[archiveNum].file[fileNum].selected = 0;
    tarArchive[archiveNum].count++;

    // Getting relative Path of file for comparison
    char relativePath[PATH_MAX];
    int fullLength = strlen(fullFilePath);
    strncpy(relativePath, fullFilePath + dirNameLength, (fullLength - dirNameLength) + 1);
    strcpy(tarArchive[archiveNum].file[fileNum].relativePath, relativePath);
}

// Opens and reads directory
// Directory details are added to directory struct
// File details are added to files struct within archive struct
void listFiles (char* dirName, int archiveNum, int dirNameLength){
    DIR* dir;

    //  Opening directory
    if ((dir = opendir(dirName)) == NULL){
        perror("opendir() failed");
        exit(EXIT_FAILURE);
    }

    //  Reading directory
    while (1){
        struct dirent* readDir;
        const char* d_name;
        
        if ((readDir = readdir(dir)) == NULL){
            break;
        }
        d_name = readDir->d_name;

        // If type is directory:
        if (readDir->d_type == DT_DIR){

            // Skipping ".." and "." directories
            if ((strcmp(d_name, "..") != 0) && (strcmp(d_name, ".") != 0)){

                // path will contain temporary extracted directory + relative file path
                char path[PATH_MAX];
                snprintf (path, PATH_MAX,"%s/%s", dirName, d_name);
                listFiles(path, archiveNum, dirNameLength);

                // dir will contain temporary output directory + relative file path
                // dir is used to replicated directory structure
                char dir[PATH_MAX];
                strcpy(dir, tempDirectories[0].tempDirPath);
                int pathLength = strlen(path);
                strncat(dir, path + dirNameLength, pathLength - dirNameLength + 1);
                
                // checking if directory already exists
                // 0 if not already in directory struct and 1 if it is
                int repeated = 0;
                for (int i = 0; i <= dirCount; i++){
                    if ((strcmp(directory[i].dirPath, dir)) == 0){
                        repeated = 1;
                    }
                }
                // Add directory if it does not already exist
                // And store its details
                if (repeated == 0){
                    struct stat statbuff;
                    strcpy(directory[dirCount].dirPath, dir);
                    if (stat(path, &statbuff) == -1){
                        perror("stat() failed");
                        exit(EXIT_FAILURE);
                    }
                    directory[dirCount].dir_mode  = statbuff.st_mode;
                    directory[dirCount].dir_uid   = statbuff.st_uid;
                    directory[dirCount].dir_gid   = statbuff.st_gid;
                    directory[dirCount].dir_mtime = statbuff.st_mtime;
                    dirCount++;
                }
            }
        // If type is not a directory, its a file  
        } else if (readDir->d_type != DT_DIR) {
            // store file details
            addFileStats(dirName, d_name, archiveNum, dirNameLength);
        }
    }
    // Closing directory
    if (closedir(dir) == -1){
        perror("closedir() failed");
        exit(EXIT_FAILURE);
    }
}

// Applies merging criteria to select output files
/* The logic behind 4 for loops is simple:
Each file in an archive (1st and 2nd 'for loop') is compared with
all the files in the other archives (3rd and 4th 'for loop).
*/
void selectFiles(void){
    // e.g. each file in archive 0...
    for (int i = 0; i < numTarFiles; i++){
        for (int file1 = 0; file1 < tarArchive[i].count; file1++){
            // ... is compared with each file in archives 1, 2, last
            for (int j = i + 1; j < numTarFiles; j++){
                for (int file2 = 0; file2 < tarArchive[j].count; file2++){
                    
                    // Compareing relative path names
                    char file1Name[PATH_MAX];
                    char file2Name[PATH_MAX];
                    strcpy(file1Name, tarArchive[i].file[file1].relativePath);
                    strcpy(file2Name, tarArchive[j].file[file2].relativePath);
                    // If two files have same path name
                    if (strcmp(file1Name, file2Name) == 0){
                        // These files are then repeated
                        tarArchive[i].file[file1].repeated = 1;
                        tarArchive[j].file[file2].repeated = 1;

                        int file1ModTime = tarArchive[i].file[file1].mtime;
                        int file2ModTime = tarArchive[j].file[file2].mtime;
                        int file1Size    = tarArchive[i].file[file1].size;
                        int file2Size    = tarArchive[j].file[file2].size;
                        int tar1ModTime  = tarArchive[i].modTime;
                        int tar2ModTime  = tarArchive[j].modTime;

                        // Checking if the first file from earlier archive
                        // has been previously selected for final tar
                        if (tarArchive[i].file[file1].selected == 1){
                            // If it was, it should be deselected,
                            // since another repeat is found
                            tarArchive[i].file[file1].selected = 0;
                        }
                            // Selecting the newer file
                        if (file1ModTime > file2ModTime){
                            tarArchive[i].file[file1].selected = 1;
                            break;
                        } else if (file2ModTime > file1ModTime){
                            tarArchive[j].file[file2].selected = 1;
                            break;
                        }
                            // If same mod time, selected the larger size
                        if (file1Size > file2Size){
                            tarArchive[i].file[file1].selected = 1;
                            break;
                        } else if (file2Size > file1Size){
                            tarArchive[j].file[file2].selected = 1;
                            break;
                        }
                            // if same size, select the newer tar file
                        if (tar1ModTime > tar2ModTime){
                            tarArchive[i].file[file1].selected = 1;
                            break;
                        } else if (tar1ModTime > tar2ModTime){
                            tarArchive[j].file[file2].selected = 1;
                            break;

                            // They are exactly the same, select the 2nd file
                        } else {
                            tarArchive[j].file[file2].selected = 1;
                            break;
                        }
                        break;
                    }
                }
            }
            // After comparing this file with all the other files in other archives,
            // if it has not been repeated, it has unique relative path name
            // and should be selected
            if (tarArchive[i].file[file1].repeated == 0){
                tarArchive[i].file[file1].selected = 1;
            }
        }
    }
}
// Copy selected files to the final output directory structure
void copyFiles(void){

    for (int i = 0; i < numTarFiles; i++){
        for (int j = 0; j < tarArchive[i].count; j++){

            // Only copy files that have been selected
            if (tarArchive[i].file[j].selected == 1){
                // Constructing the path of the output file to be created
                char output[PATH_MAX];
                strcpy(output, tempDirectories[0].tempDirPath);
                strcat(output, "/");
                strcat(output, tarArchive[i].file[j].relativePath);

                // Openning each file
                int fd1 = open(tarArchive[i].file[j].fullPath, O_RDONLY | O_NOATIME);
                int fd2 = open(output, O_WRONLY | O_CREAT | O_EXCL);
                if (fd1 == -1 || fd2 == -1){
                    perror("open() failed");
                    exit(EXIT_FAILURE);
                }
                
                // Allocating buffer size based on known file size
                void* buf = malloc(tarArchive[i].file[j].size);

                // Read() from fd1, Write to fd2
                if (read(fd1, buf, tarArchive[i].file[j].size) == -1){
                    perror("read() failed");
                    exit(EXIT_FAILURE);
                }
                if (write(fd2, buf, tarArchive[i].file[j].size) == -1){
                    perror("write() failed");
                    exit(EXIT_FAILURE);
                }

                // Matching the permissions of the copied file with the original
                if (chmod(output, tarArchive[i].file[j].f_mode) == -1){
                    perror("chmod failed");
                    exit(EXIT_FAILURE);
                }

                // Maching the last modification time of the copied file with the original
                struct timeval newtimes[2];
                newtimes[0].tv_sec     = tarArchive[i].file[j].mtime;
                newtimes[0].tv_usec    = 0;
                newtimes[1].tv_sec     = tarArchive[i].file[j].mtime;
                newtimes[1].tv_usec    = 0;
                utimes(output, newtimes);
        
                free(buf);
                close(fd1);
                close(fd2);
            }
        }
    }
}