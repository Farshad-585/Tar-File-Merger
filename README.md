# Tar File Merger
---
### CITS2002 - Systems Programming
#### The University of Western Australia

---
#### Project goals
The goal of this project was to write a command-line utility program in C99, named mergetars, which merges the contents of multiple tar archive files into a single tar archive file. This project developed my understanding of advanced features of the C99 programming language, and many useful Linux operating system system-call and POSIX function calls.

---
#### Description
A medium-sized business has decided to migrate its files to cloud-based storage, requiring it to first identify all files to migrate. A critical disk failure at the worst possible time now requires all files to be recovered from recent backups. However, the business' IT wizard has recently left for a lucrative position at a cloud-based storage company. Management has located the backups, but they have been poorly labeled, making it impossible to easily identify what is contained in each backup and when each was made. The decision has been made to migrate just the latest copy of each file to the cloud, which will require an 'intelligent merging' of the backups' contents.

The backups have been made using the widely available tar command, a well-defined file format whose name is a contraction of tape archive, reflecting the backup media with which the command was first used. While the tar command supports many actions to create, list, extract, and append tar archive files, it offers no support to merge archives together.

The business has located many backups each holding thousands of files. The task to identify all duplicate files, and to find the most recent version of similar files, is too large to be performed manually, and your team has been contracted to develop a new command-line utility program to intelligently merge all of the backups' contents into a single (large) tar archive.

---
#### Program invocation
The purpose of your mergetars command-line utility is to merge the contents of multiple tar archive files into a single tar archive. The program receives the name of one or more input filenames, and a single output filename (if only a single input filename is provided, then mergetars will act like a simple file-copying program, although there is no requirement to check for this special case).

A typical program invocation is:
```
prompt>  ./mergetars input_tarfile1 [input_tarfile2 ...] output_tarfile
```
Filenames will always end with the suffix .tar – indicating that the archive does not involve any compression – or with the suffix .tar.gz or .tgz – indicating that the archive is (or will be) compressed using the GZIP compression algorithm. The standard tar utility supports these cases using its -z command-line option. There is no requirement for mergetars to support any other compression schemes.

---
#### The merging criteria
The inputs are merged to form the output according to the following definitions and rules:

  - Two or more files from different archives are considered the same file if they have the same relative pathname (including the name of the directory holding the file and the filename itself).

  - If from different archives, the files "project2/src/mergetars.c" and "project2/src/mergetars.c" are considered the same file. In contrast, "monday/project2/src/mergetars.c" and "wednesday/project2/src/mergetars.c" are considered different files.

  - Two or more files from different archives with the same relative pathname, are considered different versions of the same file. The output archive should contain just the latest of all such versions.

  - All other files with different relative pathnames are considered different files. The output archive should contain one copy of each different file.

---
NOTE: THIS PROJECT WAS PART OF THE SYSTEMS PROGRAMMING UNIT(CITS2002) AT THE UNIVERSITY OF WESTERN AUSTRALIA. 
