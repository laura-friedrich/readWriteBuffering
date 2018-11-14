#include "myio.h"

// Include our header file

/* Accepts Flags:
- O_CREAT
- O_WRONLY
- O_RDONLY
- O_RDWR
- O_TRUNC
*/


struct FileStruct* myflush(struct FileStruct *fd){
  fd->bufferWritten = 0;
  write(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE);
  return fd;
}


//mywrite implementations
ssize_t mywrite(struct FileStruct *fd, const void *buf, size_t count){

  fd->bufferWritten = 1;
  if(BUFFER_SIZE-fd->bufferOffset > count){
    memcpy(fd->fileBuffer+fd->bufferOffset, buf, count);
    fd->bufferOffset = fd->bufferOffset+count;
  }
  else{

    int countInBuf = BUFFER_SIZE-fd->bufferOffset;
    memcpy( fd->fileBuffer+fd->bufferOffset, buf, countInBuf);

    write(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE);

    while (count>= BUFFER_SIZE){
      memcpy( fd->fileBuffer, buf, countInBuf);

      write(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE);
      count = count - BUFFER_SIZE;
      fd->beginningBuff= BUFFER_SIZE+fd->beginningBuff;
      fd->endBuff = fd->beginningBuff+ BUFFER_SIZE;
    }
    memcpy( fd->fileBuffer, buf, count);
    fd->beginningBuff= BUFFER_SIZE+fd->beginningBuff;
    fd->endBuff = fd->beginningBuff+ BUFFER_SIZE;
    fd->bufferOffset =0;

  }


  return 0;

}


// myread implementations
ssize_t myread(struct FileStruct *fd, void *buf, size_t count){



  if(BUFFER_SIZE - fd->bufferOffset >count){

    memcpy(buf, fd->fileBuffer+fd->bufferOffset, count);
    fd->bufferOffset = fd->bufferOffset+count;

  }
  else{
    int countInBuf = BUFFER_SIZE - fd->bufferOffset;

    memcpy(buf, fd->fileBuffer+fd->bufferOffset, countInBuf );
    printf("count before change is %ld \n", count);
    fflush(stdout);
    count= count-countInBuf;

    if(fd->bufferWritten==1)  {
      fd->bufferWritten = 0;
      write(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE);

    }

    while(count>=BUFFER_SIZE)  {
      read(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE);
      memcpy(buf, fd->fileBuffer, BUFFER_SIZE);
      count=count-BUFFER_SIZE;
      fd->beginningBuff= BUFFER_SIZE+fd->beginningBuff;
      fd->endBuff = fd->beginningBuff+ BUFFER_SIZE;
    }

    read(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE);
    memcpy(buf, fd->fileBuffer, count);
    fd->beginningBuff= BUFFER_SIZE+fd->beginningBuff;
    fd->endBuff = fd->beginningBuff+ BUFFER_SIZE;
    fd->bufferOffset =0;
  }
  return 0; 
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
  myflush(fd);
  return close(fd->fileDescriptor);
}
