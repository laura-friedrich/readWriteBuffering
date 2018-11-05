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

int main(int argc, char *argv[])
{
  struct FileStruct fileToTest;
  fileToTest.fileName = argv[1];
  fileToTest.position = 0;
  fileToTest.bufferWritten = 0;
  fileToTest.fileDescriptor = myopen(fileToTest.fileName, O_RDONLY);

  printf("Calling 'myopen' on %s, recieved int %d.\n",
          fileToTest.fileName,
          myopen(fileToTest.fileName, O_CREAT));
  printf("Calling 'myread' on %s, recieved ssize_t %ld.\n",
          fileToTest.fileName,
          myread(&fileToTest, fileToTest.fileBuffer, BUFFER_SIZE));
  printf("Has the buffer been written to? %d.\n", fileToTest.bufferWritten);
}

/* Accepts Flags:
  - O_CREAT
  - O_WRONLY
  - O_RDONLY
  - O_RDWR
  - O_TRUNC
*/

// myread implementations
ssize_t myread(struct FileStruct *fd, void *buf, size_t count){
  // Current buffer not adequate, read a new chunk of file of 'BUFFER_SIZE'
  if(fd->bufferWritten == 0 || count + fd->position > BUFFER_SIZE){
    printf("Calling system call read for file: %s.\n", fd->fileName);
    read(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE);
    fd->bufferWritten = 1; // Now the buffer has been written
    printf("Contents of file:\n\n%s\n\n", fd->fileBuffer);
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
