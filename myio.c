
#include "myio.h"

// Include our header file

/* Accepts Flags:
- O_CREAT
- O_WRONLY
- O_RDONLY
- O_RDWR
- O_TRUNC
*/


struct FileStruct* myflush(struct FileStruct *fd)  {
  fd->bufferWritten = 0;
  write(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE);
  return fd;
}

//mywrite implementations
ssize_t mywrite(struct FileStruct *fd, const void *buf, size_t count)  {

  fd->bufferWritten = 1;
  if(BUFFER_SIZE-fd->bufferOffset > count)  {
    memcpy(fd->fileBuffer+fd->bufferOffset, buf, count);
    fd->bufferOffset = fd->bufferOffset+count;
  }

  else  {
    int countInBuf = BUFFER_SIZE-fd->bufferOffset;
    memcpy( fd->fileBuffer+fd->bufferOffset, buf, countInBuf);
    write(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE);
    //printf("what is in the buf is: %s\n", fd->fileBuffer);


    while (count>= BUFFER_SIZE)  {
      memcpy( fd->fileBuffer, buf, countInBuf);
      write(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE);
      count = count - BUFFER_SIZE;
      fd->beginningBuff= BUFFER_SIZE+fd->beginningBuff;
      fd->endBuff = fd->beginningBuff+ BUFFER_SIZE;

      //printf("what is in the buf is: %s", fd->fileBuffer);

    }

    memcpy( fd->fileBuffer, buf, count);
    fd->beginningBuff= BUFFER_SIZE+fd->beginningBuff;
    fd->endBuff = fd->beginningBuff+ BUFFER_SIZE;
    fd->bufferOffset =0;

  }
  return 0;  //not sure what to return here
}

// myread implementations
ssize_t myread(struct FileStruct *fd, void *buf, size_t count)  {

  void *newBuf;
  if(BUFFER_SIZE - fd->bufferOffset >count)  {
    //printf("here");
    memcpy(buf, fd->fileBuffer+fd->bufferOffset, count);
    fd->bufferOffset = fd->bufferOffset+count;
  }

  else {
    //printf("NO here");
    int countInBuf = BUFFER_SIZE - fd->bufferOffset;
    memcpy(buf, fd->fileBuffer+fd->bufferOffset, countInBuf );
    //printf("%p\n", buf);
    newBuf =(long *) buf + (countInBuf/8);
    //printf("%p\n", newBuf);
    //printf("%ld\n", count);
    count= count-countInBuf;
    //printf("%ld\n", count);
    if(fd->bufferWritten==1)  {
      fd->bufferWritten = 0;
      write(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE);
    }
    while(count>=BUFFER_SIZE)  {
      read(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE);
      memcpy(newBuf, fd->fileBuffer, BUFFER_SIZE);
      newBuf = (long *)newBuf+(BUFFER_SIZE/8);
      count=count-BUFFER_SIZE;
      fd->beginningBuff= BUFFER_SIZE+fd->beginningBuff;
      fd->endBuff = fd->beginningBuff+ BUFFER_SIZE;
    }

    long readBytes = read(fd->fileDescriptor,fd->fileBuffer, BUFFER_SIZE);
    if (readBytes < count){
      count = readBytes;
    }
    memcpy(newBuf, fd->fileBuffer, count);
    fd->beginningBuff= BUFFER_SIZE+fd->beginningBuff;
    fd->endBuff = fd->beginningBuff+ BUFFER_SIZE;
    fd->bufferOffset =count;
  }
  return 0;  //not sure what to return here
}


struct FileStruct* myopen(char *fileName, int flags)  {
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

int myclose(struct FileStruct *fd)  {
  if (fd->bufferWritten ==1)  {
    myflush(fd);
  }
  int returnVal = close(fd->fileDescriptor);
  free(fd);
  return returnVal;
}
