#include "myio.h"

// Include our header file

/* Accepts Flags:
  - O_CREAT
  - O_WRONLY
  - O_RDONLY
  - O_RDWR
  - O_TRUNC
*/


//mywrite implementations
ssize_t mywrite(struct FileStruct *fd, const void *buf, size_t count){
  printf("SEGFAULT???\n");
  fflush(stdout);
  if(fd->endBuff - fd->beginningBuff+fd->bufferOffset <= count){
    memcpy(fd->fileBuffer+fd->bufferOffset, buf, count);
    fd->bufferOffset = fd->bufferOffset+count;
  }
  else{
    printf("where the fuck is my seg fault\n");
    fflush(stdout);
    int countInBuf = fd->endBuff - fd->beginningBuff - fd->bufferOffset;
    memcpy( fd->fileBuffer+fd->bufferOffset, buf, countInBuf );
    myflush(fd);
    printf("where the fuck is my seg fault\n");
    fflush(stdout);
    mywrite(fd, buf, count-countInBuf);
    printf("IS IT FUCKING HERE???\n");
    fflush(stdout);

  }
  //if (fd->bufferWritten==0 ){
    //memcpy(fd->fileBuffer, buf, count);
    //fd->bufferWritten = 1; // Now the buffer has been written
    //fd->position = count;
    //printf("Calling system call write for the file:%s. \n\n", fd->fileName);
    //write(fd->fileDescriptor, fd->fileBuffer, count);
  //}
  //else if (fd->position + count < BUFFER_SIZE){
  //  memcpy(fd->fileBuffer+fd->position, buf, count);
    //fd->position = count + fd->position;
  //}
  //else {
  //  myflush(fd);
    //mywrite(fd, buf,count);
  //}

  return 0;

}


// myread implementations
ssize_t myread(struct FileStruct *fd, void *buf, size_t count){
  // Current buffer not adequate, read a new chunk of file of 'BUFFER_SIZE'

  printf("myread no segfault 0\n");
  fflush(stdout);

  if(fd->endBuff - fd->beginningBuff+fd->bufferOffset <= count){
    //printf("Calling system call read for file: %s.\n\n", fd->fileName);
    //read(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE);
    printf("myread no segfault 1");
    memcpy(buf, fd->fileBuffer+fd->bufferOffset, count);
    fd->bufferOffset = fd->bufferOffset+count;
    //fd->bufferWritten = 1; // Now the buffer has been written
    //memcpy(buf, fd->fileBuffer, count);
  }
  else{
    int countInBuf = fd->endBuff - fd->beginningBuff - fd->bufferOffset;
    printf("myread no segfault 2\n");
    memcpy(buf, fd->fileBuffer+fd->bufferOffset, countInBuf );
    if(fd->bufferWritten==1){
      myflush(fd);
    }
    read(fd->fileDescriptor, fd->fileBuffer+fd->beginningBuff, BUFFER_SIZE);
    fd->beginningBuff= BUFFER_SIZE+fd->beginningBuff;
    fd->endBuff = fd->beginningBuff+ BUFFER_SIZE;
    fd->bufferOffset =0;

    //myread(fd, buf, count-countInBuf);
  }
  //else{
    //memcpy(buf, fd->fileBuffer+fd->position, count);
    //fd->position = fd->position+count;
  //}
  //memcpy to transfer into buf
  printf("NOSEGFAULT\n");
  fflush(stdout);

  return 0; //Not always return 0. Return a different number based on success or fail
}

struct FileStruct* myflush(struct FileStruct *fd){
  fd->bufferWritten = 0;
  write(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE);
  return fd;
}

// Calls system call open
struct FileStruct* myopen(char *fileName, int flags){
  struct FileStruct *fileStruct = malloc(sizeof(struct FileStruct));
  fileStruct->fileName = fileName;
  fileStruct->bufferOffset = 0;
  fileStruct->bufferWritten = 0;
  fileStruct->beginningBuff=0;
  fileStruct->endBuff=BUFFER_SIZE;
  fileStruct->flags = flags;
  fileStruct->fileDescriptor = open(fileName, flags);
  read(fileStruct->fileDescriptor, fileStruct->fileBuffer, BUFFER_SIZE);
  return fileStruct;
}

// Calls system call close
int myclose(struct FileStruct *fd){
  return close(fd->fileDescriptor);
}
