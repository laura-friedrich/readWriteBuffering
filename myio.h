#ifndef __MYIO_H
#define __MYIO_H
#define BUFFER_SIZE 1024

typedef struct FileStruct{
    char *fileName;
    char fileBuffer[BUFFER_SIZE];
    int bufferWritten; // Keeps track of if buffer has been filled or not
    int fileDescriptor;
    int position;
}FileStruct;

//int myopen(struct File fd, int flags);
int myopen(const char *pathname, int flags);

int myclose(int fd);

//ssize_t myread(int fd, void *buf, size_t count);
ssize_t myread(struct FileStruct *fd, void *buf, size_t count);

#endif
