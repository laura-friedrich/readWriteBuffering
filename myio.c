#include <sys/types.h>
#include <stdio.h>
// Needed for open
#include <fcntl.h>
#include <sys/stat.h>
// Needed for close, read
#include <unistd.h>
#include <stdlib.h>
// Include our header file
#include "myio.h"

#define BUFFER_SIZE 1024


struct FileStruct{
  char *fileName;
  char *fileBuffer[BUFFER_SIZE];
  int fileDescriptor;
  int startByte, endByte;
};

int main(int argc, char *argv[])
{
  //char *read_buffer = (char*) malloc (BUFFER_SIZE);
  struct FileStruct fileToTest;
  fileToTest.fileName = argv[1];
  fileToTest.startByte = 0;
  fileToTest.endByte = 0;
  fileToTest.fileDescriptor = 0;
  fileToTest.fileDescriptor = myopen(fileToTest.fileName, O_RDONLY);

  printf("%d\n", myclose(myopen(fileToTest.fileName, O_CREAT)));
  printf("%ld\n", myread(fileToTest, fileToTest.fileBuffer, BUFFER_SIZE));

  //printf("%s\n", contents);
}

/* Accepts Flags:
  - O_CREAT
  - O_WRONLY
  - O_RDONLY
  - O_RDWR
  - O_TRUNC
*/

// myread implementations
ssize_t myread(struct FileStruct fd, void *buf, size_t count){
  // Current buffer not adequate, read a new chunk of file of 'BUFFER_SIZE'
  if(fd.fileBuffer == NULL || (fd.endByte - fd.startByte) < BUFFER_SIZE){
    printf("Calling system call read for file:\n%s", fd.fileName);
    read(fd.fileDescriptor, fd.fileBuffer, BUFFER_SIZE);
    printf("Contents of file: %s\n", fd.fileBuffer);
  }

  return 0;
}

// Calls system call open
int myopen(const char *fileName, int flags){
  return open(fileName, flags);
}

// Calls system call close
int myclose(int fd){
  return close(fd);
}
