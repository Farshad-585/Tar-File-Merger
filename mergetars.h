//  CITS2002 Project 2 2020
//  Name:             Farshad Ghanbari
//  Student number:   21334883

#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<time.h>
#include<errno.h>
#include<limits.h>
#include<fcntl.h>
#include<sys/time.h>

#define TEMPLATE    "/tmp/temp-XXXXXX"

/*
    NOTE: my initial realloc() implementation was causing a small bug.
    I could not figure out what the problem was even after discussing it
    in the lab. So I had to change from dynamic allocation to constant
    number in the below cases.
*/
#define dirMAX      10000               // max number of unique directory paths
#define maxFiles    50000               // max number of files per archive

//  GLOBAL FUNCTIONS
extern void initialiseGlobals(int);     // Performs initialisation of global variables
extern void tempDirCreate(char*);       // Creates temporary directories using the TEMPLATE
extern void tarExpand(char*[]);         // Extracts tar files
extern void listFiles (char*, int, int);// Stores details of files in each archive
extern void selectFiles(void);          // Applies merging criteria
extern void dirTreeCreate(void);        // Creates final output directory tree
extern void copyFiles(void);            // Copy files using open() read() write() close()
extern void tarCreate(char*);           // Creates final tar file
extern void dirRemove(void);            // Cleans up temporary directories and free() memory

//  GLOBAL VARIABLES
extern int numTarFiles;                 // Number of tar files provided in argument
extern int dirCount;                    // Total number of directories stored

//  DATA STRUCTURES
typedef struct files{
    char fullPath[PATH_MAX];            // Full file path of the file
    char relativePath[PATH_MAX];        // Path after temporary directory
    mode_t f_mode;                      // stat() st_mode
    int size;                           // stat() st_size
    int mtime;                          // stat() st_mtime
    int selected;                       // 0 if not selected; 1 if selected
    int repeated;                       // 0 if not repeated; 1 if repeated
} files;
extern files file[maxFiles];            // file variable only used in sizeof() for malloc

typedef struct archive{
    int modTime;                        // stat() st_mtime
    int count;                          // Number of files in this archive
    files file[maxFiles];               // files types structures
} archive;
extern archive* tarArchive; 

typedef struct dir{
    char dirPath[PATH_MAX];             // Full path of the directory to be created
    int created;                        // 0 if not created; 1 if created
    mode_t dir_mode;                    // stat() st_mode
    uid_t dir_uid;                      // stat() st_uid   
    gid_t dir_gid;                      // stat() st_gid
    int dir_mtime;                      // stat() st_mtime
} dir;
extern dir directory[];                  // NOTE: stats are stored, but not implemented in this version

typedef struct tempDirs{
    char tempDirPath[PATH_MAX];         // Path of temporary directory
    int count;                          // Total number of temp directories created
} tempDirs;
extern tempDirs* tempDirectories;       // Used to remove temp dirs