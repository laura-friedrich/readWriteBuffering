
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
    //perror("write");
    exit(1);
  }
  fd->beginningBuff= fd->bufferOffset+fd->beginningBuff;
  return fd;
}

//mywrite implementations
ssize_t mywrite(struct FileStruct *fd, const void *buf, size_t count)  {
  fd->bytesWritten=0;
  if (fd->bufferLoaded == 0){
    if (read(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE)==-1){
      //I dont understand how to avoid reading here because I need to potentialll
      //read or write so I am preemptivly reading into my buffer so if i dont change the whole
      //buffer size, I can still write the whole buffer to the file at the end
      //perror("read");
      exit(1);
    }
    fd->bufferLoaded = 1;
  }
  void *newBuf;
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
      //perror("write");
      exit(1);
    }

    fd->beginningBuff= BUFFER_SIZE + fd->beginningBuff;
    count=count - countInBuf;
    newBuf =(long *) buf + (countInBuf/8);

    while (count >= BUFFER_SIZE)  {
      memcpy( fd->fileBuffer, newBuf,BUFFER_SIZE);
      fd->bytesWritten= fd->bytesWritten + BUFFER_SIZE;
      if (write(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE) == -1){
        //perror("write");
        exit(1);
      }
      newBuf = (long *)newBuf + (BUFFER_SIZE/8);
      count = count - BUFFER_SIZE;
      fd->beginningBuff= BUFFER_SIZE + fd->beginningBuff;
      fd->endBuff = fd->beginningBuff + BUFFER_SIZE;
    }
    memcpy( fd->fileBuffer, newBuf, count);
    fd->beginningBuff = BUFFER_SIZE + fd->beginningBuff;
    fd->bufferOffset = count;
    fd->bytesWritten= fd->bytesWritten + count;
  }
  return fd->bytesWritten;
}

// myread implementations
ssize_t myread(struct FileStruct *fd, void *buf, size_t count)  {
  //how do i count bytes? becausewhen I call read, I get how many bytes
  //were read but I call read preemptivly thus??? how do i count??
  fd->bytesRead=0;
  if (fd->bufferLoaded == 0){
    if (read(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE)==-1){
      //perror("read");
      exit(1);
    }
    fd->bufferLoaded = 1;
  }
  void *newBuf;
  if(BUFFER_SIZE - fd->bufferOffset >count)  {
    memcpy(buf, fd->fileBuffer +  fd->bufferOffset, count);
    fd->bufferOffset = fd->bufferOffset+count;
    fd->bytesRead = count;
  }

  else {
    int countInBuf = BUFFER_SIZE - fd->bufferOffset;
    memcpy(buf, fd->fileBuffer + fd->bufferOffset, countInBuf );
    fd->bytesRead = countInBuf;
    newBuf =(long *) buf + (countInBuf/8);
    count= count - countInBuf;
    if(fd->bufferWritten == 1)  {
      fd->bufferWritten = 0;
      if (write(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE)==-1){
        //perror("write");
        exit(1);
      }
    }
    while(count>=BUFFER_SIZE)  {
      if (read(fd->fileDescriptor, fd->fileBuffer, BUFFER_SIZE)==-1){
        perror("read");
        exit(1);
      }
      memcpy(newBuf, fd->fileBuffer, BUFFER_SIZE);
      newBuf = (long *)newBuf + (BUFFER_SIZE/8);
      count=count - BUFFER_SIZE;
      fd->beginningBuff= BUFFER_SIZE + fd->beginningBuff;
      fd->bytesRead = BUFFER_SIZE + fd->bytesRead;
    }

    long readBytes = read(fd->fileDescriptor,fd->fileBuffer, BUFFER_SIZE);
    if (readBytes == -1){
      //perror("read");
      exit(1);
    }
    if (readBytes < count){
      count = readBytes;
    }
    memcpy(newBuf, fd->fileBuffer, count);
    fd->bytesRead = count + fd->bytesRead;
    fd->beginningBuff= BUFFER_SIZE + fd->beginningBuff;
    fd->bufferOffset = count;
  }
  return fd->bytesRead;
}


struct FileStruct* myopen(char *fileName, int flags)  {
  struct FileStruct *fileStruct = malloc(sizeof(struct FileStruct));
  if (fileStruct == NULL){
    //perror("malloc");
    exit(1);
  }
  fileStruct->fileName = fileName;
  fileStruct->bufferOffset = 0;
  fileStruct->bufferWritten = 0;
  fileStruct->beginningBuff=BUFFER_SIZE;
  fileStruct->bytesRead = 0;
  fileStruct->bufferLoaded = 0;
  fileStruct->bytesWritten =0;
  fileStruct->flags = flags;
  fileStruct->fileDescriptor = open(fileName, flags, 0666);
  if (fileStruct->fileDescriptor == -1) {
    free(fileStruct);
    //perror("open");
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
    //perror("close");
    exit(1);
  }
  free(fd);
  return returnVal;
}
