# -*- Makefile -*-
#  CITS2002 Project 2 2020
#  Name:             Farshad Ghanbari
#  Student number:   21334883

mergetars : mergetars.o globals.o tarManage.o fileManage.o dirManage.o
	cc -std=c99 -Wall -pedantic -Werror -o mergetars \
		mergetars.o globals.o tarManage.o fileManage.o dirManage.o

mergetars.o : mergetars.c mergetars.h
	cc -std=c99 -Wall -pedantic -Werror -c mergetars.c
tarManage.o : tarManage.c mergetars.h
	cc -std=c99 -Wall -pedantic -Werror -c tarManage.c
globals.o : globals.c mergetars.h
	cc -std=c99 -Wall -pedantic -Werror -c globals.c 
fileManage.o : fileManage.c mergetars.h
	cc -std=c99 -Wall -pedantic -Werror -c fileManage.c	
dirManage.o : dirManage.c mergetars.h
	cc -std=c99 -Wall -pedantic -Werror -c dirManage.c	
	
clean:
	rm -f *.o mergetars