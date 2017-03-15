// #include "fileReader.h"

// // /*
// //     true if the file is open
// // */
// // int isFileOpen(FileInfo* info) {
// //     return !feof(info->file);
// // }

// // reading part
// /* load xxx , return the name of the file to load
//    true if this is a loading command.
// */
// int parseLoadFileName(FileInfo* info, char* buffer) {
//     if (strncmp(buffer, "load ", 5) == 0) {
//         strcpy(info->name, buffer+5);
//         info->file = fopen(info->name, "r");
//         return 1;
//     }
//     return 0;
// }

// // return the bits representing the file
// int readFile(FileInfo* fileInfo, char* buffer, int maxSize) {
//     int numbytes = fread(buffer, 1, maxSize, fileInfo->file);
//     return numbytes;
// }


// /*
//     check if the data contains file info.
//     write the file if it is the file.
//     close of file if it is a regular message.
// */
// int checkForFile(FileInfo* info, char* data, int size) {
//     if (data[0] == 0) {
//         strcpn(info->name, data+2);
//     }
//     return 0;
// }

// // return the bits representing the file
// int writeFile(FileInfo* fileInfo, char* data, int size) {
//     fwrite(data+2, 1, size-2, info->file);
//     return data[1];
// }



