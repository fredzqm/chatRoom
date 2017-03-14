#include <stdio.h>
#include <string.h>

typedef struct 
{
	char name[100];
	FILE* file;
} FileInfo;


/*
    true if the file is open
*/
// int isFileOpen(FileInfo* info);

// reading part
/* load xxx , return the name of the file to load
   true if this is a loading command.
*/
int parseLoadFileName(FileInfo* info, char* str);

// return the bits representing the file
int readFile(FileInfo* fileInfo, char* buffer, int maxSize);


/*
    check if the data contains file info.
    write the file if it is the file.
    close of file if it is a regular message.
*/
int checkForFile(FileInfo* info, char* data, int size);

// return the bits representing the file
int writeFile(FileInfo* fileInfo, char* data, int size);


