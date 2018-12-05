
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
  //int writeVal = fd->bytesWritten%BUFFER_SIZE;
  //printf("%s\n", fd->fileBuffer);
  //printf("%d\n", writeVal );
  //printf("here4\n");
  //int printVal = write(fd->fileDescriptor, fd->fileBuffer, writeVal);
  //printf("bytes being written in myflush ar %d\n", printVal);
//printf("here5\n");
  //if(printVal==-1){

  //if (write(fd->fileDescriptor, fd->fileBuffer, writeVal)==-1){
  //printf("here%d\n", fd->bufferOffset);
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
    //printf("fd->bytes read at beginning of myWrite is %d\n", fd->bytesLeftInBuffer);
    //printf("file buffer here is%s\n", fd->fileBuffer);
    if (fd->bytesLeftInBuffer==-1){
      fd->error = 3;
    }

    fd->bufferLoaded = 1;
    lseek(fd->fileDescriptor, 0, SEEK_SET);
  }
  //void *newBuf;
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
    //printf("here1\n");
    if (write(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE)==-1){
      fd->error = 3;
    }
    fd->bufferWritten = 0; //New line might not work

    fd->beginningBuff= BUFFER_SIZE + fd->beginningBuff;
    count=count - countInBuf;
    buf =(char *) buf + countInBuf;

    while (count >= BUFFER_SIZE)  {

      memcpy( fd->fileBuffer, buf,BUFFER_SIZE);

      fd->bytesWritten= fd->bytesWritten + BUFFER_SIZE;
      //printf("here2\n");
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
  //how do i count bytes? becausewhen I call read, I get how many bytes
  //were read but I call read preemptivly thus??? how do i count?
  //int countInitial = count;
  int returnVal;
  if (fd->bufferLoaded == 0){
    fd->bytesLeftInBuffer = read(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE);
    //printf("%d\n", fd->bytesLeftInBuffer);
    fd->bufferOffset = 0;
    if (fd->bytesLeftInBuffer==-1){
      fd->error = 2;
    }
    fd->bufferLoaded = 1;
  }
  //void *newBuf;
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
    int countInBuf = BUFFER_SIZE - fd->bufferOffset;
    if (countInBuf>fd->bytesLeftInBuffer){
      countInBuf = fd->bytesLeftInBuffer;
      count=0;
    }
    memcpy(buf, fd->fileBuffer + fd->bufferOffset, countInBuf );
    returnVal=countInBuf;
    fd->bytesLeftInBuffer = 0; //this is wrong  change to fd->bytesRemaininBuf
    buf =(char *) buf + countInBuf;
    count= count - countInBuf;
    //printf("%s\n", fd->fileBuffer);
    if(fd->bufferWritten == 1 && fd->bufferOffset !=0)  {
      fd->bufferWritten = 0;
      //printf("here3\n");
      fflush(stdout);

      //if (write(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE)==-1){
      if (write(fd->fileDescriptor, fd->fileBuffer, fd->bufferOffset)==-1){
        //perror("write");
        fd->error = 2;
      }
    }
    while(count>=BUFFER_SIZE)  {
      //printf("hereeeeeeee\n");
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
      //fd->beginningBuff= BUFFER_SIZE + fd->beginningBuff;
      //fd->bytesLeftInBuffer = BUFFER_SIZE + fd->bytesLeftInBuffer;
    }

    fd->bytesLeftInBuffer = read(fd->fileDescriptor,fd->fileBuffer, BUFFER_SIZE);
    if (fd->bytesLeftInBuffer == -1){
      //perror("read");
      fd->error = 2;
    }
    if (fd->bytesLeftInBuffer < count){
      count = fd->bytesLeftInBuffer;
    }
    memcpy(buf, fd->fileBuffer, count);
    returnVal = returnVal + count;
    ///fd->bytesLeftInBuffer = count + fd->bytesLeftInBuffer;
    //fd->beginningBuff= BUFFER_SIZE + fd->beginningBuff;
    fd->bufferOffset = count;
  }
  //printf("return val is %d", returnVal);
  if (fd->bytesLeftInBuffer != 0){
  //if (returnVal%BUFFER_SIZE != 0){
    //lseek()
    //printf("return val is %d", returnVal);
    //printf("here\n");
    lseek(fd->fileDescriptor, (-1 * BUFFER_SIZE), SEEK_CUR);
  }
  //lseek(with countinitial if countinitial is > returnVal)
  if (fd->error == 2){
    fd->error = 0;
    return 2;
  }
  else{
    return returnVal;
  }
}


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
