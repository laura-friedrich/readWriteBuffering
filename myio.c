#include "myio.h"

// Include our header file

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
    memcpy(fd->fileBuffer, buf, count);
  }
  //memcpy to transfer into buf
  return 0; //Not always return 0. Return a different number based on success or fail
}

// Calls system call open
struct FileStruct* myopen(char *fileName, int flags){
  struct FileStruct *fileStruct = malloc(sizeof(struct FileStruct));
  fileStruct->fileName = fileName;
  fileStruct->position = 0;
  fileStruct->bufferWritten = 0;
  fileStruct->flags = flags;
  fileStruct->fileDescriptor = open(fileName, flags);
  return fileStruct;
}

// Calls system call close
int myclose(int fd){
  return close(fd);
}
