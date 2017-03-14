#include <stdio.h>

// reading part
/* load xxx */
void parseLoadFileName(char* str);

/* 1 if there is still file to read */
int isLaodingFile();

// return the bits representing the file
int readFile(char* buffer, int maxSize);

// writing part
// check if the data recieved is a file
int isFile(char* data, int size);

// write the file
void writeFile(char* data, int size);





