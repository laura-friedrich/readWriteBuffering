
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
  if (write(fd->fileDescriptor, fd->fileBuffer, fd->bufferOffset)==-1){
    perror("write");
  }
  fd->beginningBuff= fd->bufferOffset+fd->beginningBuff;
  return fd;
}

//mywrite implementations
ssize_t mywrite(struct FileStruct *fd, const void *buf, size_t count)  {
  if (fd->bufferLoaded == 0){
    if (read(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE)==-1){
      perror("read");
    }
    fd->bufferLoaded = 1;
  }
  void *newBuf;
  fd->bufferWritten = 1;

  if(BUFFER_SIZE - fd->bufferOffset > count)  {
    //printf("here\n" );
    //printf("buffer offset %d\n",fd->bufferOffset );
    memcpy(fd->fileBuffer + fd->bufferOffset, buf, count);
    fd->bufferOffset = fd->bufferOffset + count;
    //printf("here\n");
  }

  else  {
    int countInBuf = BUFFER_SIZE - fd->bufferOffset;
    //printf("count in buf\n %d", countInBuf);
    //printf("%d\n",countInBuf );
    //char* hello = fd->fileBuffer + fd->bufferOffset/8;
    //printf("%s\n", hello);
    //printf(" count in buf else case %d\n",countInBuf);
    //printf("count is %ld\n", count );
    //newBuf =(long *) buf + (fd->bufferOffset/8);
    //memcpy( newBuf, buf, countInBuf);
    memcpy( fd->fileBuffer + fd->bufferOffset, buf, countInBuf);
    //printf("here2\n");
    if (write(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE)==-1){
      perror("write");
    }
    fd->beginningBuff= BUFFER_SIZE + fd->beginningBuff;
    count=count - countInBuf;
    printf("count is %ld\n", count );
    newBuf =(long *) buf + (countInBuf/8);
    //printf("what is in the buf is: %s\n", fd->fileBuffer);


    while (count >= BUFFER_SIZE)  {
      //printf("count is %ld\n", count);
      //printf("here?????\n %ld", count);
      memcpy( fd->fileBuffer, newBuf,BUFFER_SIZE);
      if (write(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE) == -1){
        perror("write");
      }
      newBuf = (long *)newBuf + (BUFFER_SIZE/8);
      count = count - BUFFER_SIZE;
      fd->beginningBuff= BUFFER_SIZE + fd->beginningBuff;
      fd->endBuff = fd->beginningBuff + BUFFER_SIZE;

      //printf("what is in the buf is: %s", fd->fileBuffer);

    }
    //printf("coun t is  in last area %ld\n", count);
    //printf("here!\n");
    memcpy( fd->fileBuffer, newBuf, count);
    //memcpy( fd->fileBuffer+fd->bufferOffset, newBuf, count);
    fd->beginningBuff = BUFFER_SIZE + fd->beginningBuff;
    fd->bufferOffset = count;
    //myflush(fd);

  }
  return 0;  //not sure what to return here
}

// myread implementations
ssize_t myread(struct FileStruct *fd, void *buf, size_t count)  {
  if (fd->bufferLoaded == 0){
    if (read(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE)==-1){
      perror("read");
    }
    fd->bufferLoaded = 1;
  }
  void *newBuf;
  if(BUFFER_SIZE - fd->bufferOffset >count)  {
    //printf("here");
    memcpy(buf, fd->fileBuffer +  fd->bufferOffset, count);
    fd->bufferOffset = fd->bufferOffset+count;
    fd->bytesRead = count;
  }

  else {
    //printf("NO here");
    int countInBuf = BUFFER_SIZE - fd->bufferOffset;
    memcpy(buf, fd->fileBuffer + fd->bufferOffset, countInBuf );
    fd->bytesRead = countInBuf;
    //printf("%p\n", buf);
    newBuf =(long *) buf + (countInBuf/8);
    //printf("%p\n", newBuf);
    //printf("%ld\n", count);
    count= count - countInBuf;
    //printf("%ld\n", count);
    if(fd->bufferWritten == 1)  {
      fd->bufferWritten = 0;
      if (write(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE)==-1){
        perror("write");
      }
    }
    while(count>=BUFFER_SIZE)  {
      if (read(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE)==-1){
        perror("read");
      }
      memcpy(newBuf, fd->fileBuffer, BUFFER_SIZE);
      newBuf = (long *)newBuf + (BUFFER_SIZE/8);
      count=count - BUFFER_SIZE;
      fd->beginningBuff= BUFFER_SIZE + fd->beginningBuff;
      fd->bytesRead = BUFFER_SIZE + fd->bytesRead;
      //fd->endBuff = fd->beginningBuff+ BUFFER_SIZE;
    }

    long readBytes = read(fd->fileDescriptor,fd->fileBuffer, BUFFER_SIZE);
    if (readBytes == -1){
      perror("read");
    }
    if (readBytes < count){
      count = readBytes;
    }
    memcpy(newBuf, fd->fileBuffer, count);
    fd->bytesRead = count + fd->bytesRead;
    fd->beginningBuff= BUFFER_SIZE + fd->beginningBuff;
    //fd->endBuff = fd->beginningBuff+ BUFFER_SIZE;
    fd->bufferOffset = count;
  }
  return fd->bytesRead;
}


struct FileStruct* myopen(char *fileName, int flags)  {
  struct FileStruct *fileStruct = malloc(sizeof(struct FileStruct));
  if (fileStruct == NULL){
    perror("malloc");
  }
  fileStruct->fileName = fileName;
  fileStruct->bufferOffset = 0;
  fileStruct->bufferWritten = 0;
  fileStruct->beginningBuff=BUFFER_SIZE;
  fileStruct->bytesRead = 0;
  fileStruct->bufferLoaded = 0;
//  fileStruct->endBuff=BUFFER_SIZE;
  fileStruct->flags = flags;
  fileStruct->fileDescriptor = open(fileName, flags, 0666);
  if (fileStruct->fileDescriptor == -1) {
    perror("open");
  }
  //read(fileStruct->fileDescriptor, fileStruct->fileBuffer, BUFFER_SIZE);

  return fileStruct;
}

int myclose(struct FileStruct *fd)  {
  if (fd->bufferWritten ==1)  {
    myflush(fd);
  }
  int returnVal = close(fd->fileDescriptor);
  if (returnVal == -1){
    perror("close");
    printf("file name %s", fd->fileName);
  }
  free(fd);
  return returnVal;
}
