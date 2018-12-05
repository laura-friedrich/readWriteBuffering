// Import all necissary libraries for myio
#include <stddef.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
// Needed for open

#include <fcntl.h>
#include <sys/stat.h>
// Needed for close, read
#include <unistd.h>
#include <stdlib.h>
// Needed for memcpy
#include <string.h>

#ifndef __MYIO_H
#define __MYIO_H
#define BUFFER_SIZE 1024

typedef struct FileStruct{
    char *fileName;
    char fileBuffer[BUFFER_SIZE];
    int bufferWritten; // Keeps track of if buffer has been filled or not
    int fileDescriptor;
    int beginningBuff;
    int endBuff;
    int bufferOffset;
    int flags;
    int bytesRead;
    int bufferLoaded;
    int bytesWritten;
    int error;
}FileStruct;

//int myopen(struct File fd, int flags);
struct FileStruct* myopen(char *pathname, int flags);

int myclose(struct FileStruct *fd);
struct FileStruct* myflush(struct FileStruct *fd);

//ssize_t myread(int fd, void *buf, size_t count);
ssize_t myread(struct FileStruct *fd, void *buf, size_t count);

ssize_t mywrite(struct FileStruct *fd, const void *buf, size_t count);

#endif
