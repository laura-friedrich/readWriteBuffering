#include "myio.h"

int main(int argc, char *argv[])


{
  //fflush(stdout);
  void *bufferToReadTo = malloc(4096);
  void *bufferToReadTo2 = malloc(4096);

  //Have two open files at once

  //opening README
  FileStruct *fileToTest = myopen("README", O_RDONLY);

  //Opening myio.h
  FileStruct *fileToTest2 = myopen("myio.c", O_RDONLY);

  //Opening Test file to write to
  FileStruct *fileToTest3 = myopen("test", O_RDWR);


  //Reading README
  myread(fileToTest, bufferToReadTo, 100);
  //printf("file contents %s \n\n\n", (char *)bufferToReadTo);

  
  //continue reading from the file but without calling another system call
  myread(fileToTest, (char *)bufferToReadTo+100, 100);
  //printf("file contents %s \n\n\n", (char *)bufferToReadTo);



  //Reading myio.h
  myread(fileToTest2, bufferToReadTo2, 2000);

  //continue reading from the file but without calling another system call
  myread(fileToTest2, (char *)bufferToReadTo2 + 100,100);
  printf("file contents %s \n", (char *)bufferToReadTo2);

  mywrite(fileToTest3, bufferToReadTo2,2000);

  myclose(fileToTest);
  myclose(fileToTest2);
  myclose(fileToTest3);


  //free both buffers
  free(bufferToReadTo);
  free(bufferToReadTo2);






  //printf("Calling 'myread' on %s, recieved ssize_t %ld.\n",
    //      fileToTest.fileName,
    //      myread(&fileToTest, fileToTest.fileBuffer, BUFFER_SIZE));
  //printf("Has the buffer been written to? %d.\n", fileToTest.bufferWritten);
}
