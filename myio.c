
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
    fd->error = 3;
  }
  fd->beginningBuff= fd->bufferOffset+fd->beginningBuff;
  return fd;
}

//mywrite implementations
ssize_t mywrite(struct FileStruct *fd, const void *buf, size_t count)  {

  fd->bytesWritten=0;
  if (fd->bufferLoaded == 0){
    fd->bytesLeftInBuffer =read(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE);
    if (fd->bytesLeftInBuffer==-1){
      fd->error = 3;
    }
    fd->bufferLoaded = 1;
    lseek(fd->fileDescriptor, 0, SEEK_SET);
  }
  fd->bufferWritten = 1;

  if(BUFFER_SIZE - fd->bufferOffset > count)  {
    memcpy(fd->fileBuffer + fd->bufferOffset, buf, count);
    fd->bufferOffset = fd->bufferOffset + count;
    fd->bytesWritten=count;
  }

  else  {
    int countInBuf = BUFFER_SIZE - fd->bufferOffset;
    memcpy( fd->fileBuffer + fd->bufferOffset, buf, countInBuf);
    fd->bytesWritten=countInBuf;

    if (write(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE)==-1){
      fd->error = 3;
    }
    fd->lseekVal = 0;
    fd->bufferWritten = 0;
    fd->beginningBuff= BUFFER_SIZE + fd->beginningBuff;
    count=count - countInBuf;
    buf =(char *) buf + countInBuf;

    while (count >= BUFFER_SIZE)  {
      memcpy( fd->fileBuffer, buf,BUFFER_SIZE);
      fd->bytesWritten= fd->bytesWritten + BUFFER_SIZE;
      if (write(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE) == -1){
        fd->error = 3;
      }
      buf = (char *)buf + BUFFER_SIZE;
      count = count - BUFFER_SIZE;
      fd->beginningBuff= BUFFER_SIZE + fd->beginningBuff;
      fd->endBuff = fd->beginningBuff + BUFFER_SIZE;
    }
    memcpy( fd->fileBuffer, buf, count);
    fd->bufferWritten = 1;//This might not be right...
    fd->beginningBuff = BUFFER_SIZE + fd->beginningBuff;
    fd->bufferOffset = count;
    fd->bytesWritten= fd->bytesWritten + count;
  }
  if (fd->error == 3){
    fd->error = 0;
    return 3;
  }
  else{
    return fd->bytesWritten;
  }
}

// myread implementations
ssize_t myread(struct FileStruct *fd, void *buf, size_t count)  {
  int returnVal;
  if (fd->bufferLoaded == 0){
    fd->bytesLeftInBuffer = read(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE);
    fd->bufferOffset = 0;
    if (fd->bytesLeftInBuffer==-1){
      fd->error = 2;
    }
    fd->bufferLoaded = 1;
  }
  if(BUFFER_SIZE - fd->bufferOffset >count)  {
    if (count>fd->bytesLeftInBuffer) {
      count = fd->bytesLeftInBuffer;
    }
    memcpy(buf, fd->fileBuffer +  fd->bufferOffset, count);
    returnVal = count;
    fd->bufferOffset = fd->bufferOffset+count;
    fd->bytesLeftInBuffer = fd->bytesLeftInBuffer - count;
  }
  else {
    if (fd->lseekVal ==1){
      fd->lseekVal=0;
      lseek(fd->fileDescriptor,BUFFER_SIZE, SEEK_CUR);
    }
    int countInBuf = BUFFER_SIZE - fd->bufferOffset;
    memcpy(buf, fd->fileBuffer + fd->bufferOffset, countInBuf );
    returnVal=countInBuf;
    fd->bytesLeftInBuffer = 0;
    buf =(char *) buf + countInBuf;
    count= count - countInBuf;
    if(fd->bufferWritten == 1 && fd->bufferOffset !=0)  {
      fd->bufferWritten = 0;
      if (write(fd->fileDescriptor, fd->fileBuffer, fd->bufferOffset)==-1){
        fd->error = 2;
      }
    }
    while(count>=BUFFER_SIZE)  {
      fd->bytesLeftInBuffer = read(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE);
      if (fd->bytesLeftInBuffer==-1){
        fd->error = 2;
      }
      memcpy(buf, fd->fileBuffer, fd->bytesLeftInBuffer);
      returnVal = returnVal+fd->bytesLeftInBuffer;
      buf = (char *)buf + fd->bytesLeftInBuffer;
      if(fd->bytesLeftInBuffer==0){
        count = 0;
      }
      else{
        count = count - fd->bytesLeftInBuffer;
      }
    }
    fd->bytesLeftInBuffer = read(fd->fileDescriptor,fd->fileBuffer, BUFFER_SIZE);
    if (fd->bytesLeftInBuffer == -1){
      fd->error = 2;
    }
    if (fd->bytesLeftInBuffer < count){
      count = fd->bytesLeftInBuffer;
    }
    memcpy(buf, fd->fileBuffer, count);
    returnVal = returnVal + count;
    fd->bufferOffset = count;
  }
  if (fd->bytesLeftInBuffer != 0){
    lseek(fd->fileDescriptor, (-1 * BUFFER_SIZE), SEEK_CUR);
    fd->lseekVal = 1; //not sure about this
  }
  if (fd->error == 2){
    fd->error = 0;
    return 2;
  }
  else{
    return returnVal;
  }
}

//implementation of myopen
struct FileStruct* myopen(char *fileName, int flags)  {
  struct FileStruct *fileStruct = malloc(sizeof(struct FileStruct));
  if (fileStruct == NULL){
    exit(1);
  }
  fileStruct->fileName = fileName;
  fileStruct->bufferOffset = 0;
  fileStruct->bufferWritten = 0;
  fileStruct->beginningBuff=BUFFER_SIZE;
  fileStruct->bytesLeftInBuffer = 0;
  fileStruct->bufferLoaded = 0;
  fileStruct->bytesWritten =0;
  fileStruct->flags = flags;
  fileStruct->fileDescriptor = open(fileName, flags, 0666);
  fileStruct->error = 0;
  fileStruct->lseekVal = 0;
  if (fileStruct->fileDescriptor == -1) {
    free(fileStruct);
    exit(1);
  }
    return fileStruct;
}
int myclose(struct FileStruct *fd)  {
  if (fd->bufferWritten ==1)  {
    myflush(fd);
  }
  int returnVal = close(fd->fileDescriptor);
  if (returnVal == -1){
    fd->error = 4;
  }
  free(fd);
  return returnVal;
}
