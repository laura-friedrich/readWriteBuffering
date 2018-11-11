#include "myio.h"

int main(int argc, char *argv[])


{
  void *bufferToReadTo = malloc(4096);
  void *bufferToReadTo2 = malloc(4096);

  //opening README
  FileStruct *fileToTest = myopen("README", O_RDONLY);
  //printf("Calling 'myopen' on %s.\n", fileToTest->fileName);


  //Opening myio.h
  FileStruct *fileToTest2 = myopen("myio.h", O_RDONLY);
  printf("Calling 'myopen' on %s.\n", fileToTest2->fileName);

  //Reading README
  myread(fileToTest, bufferToReadTo, 100);
  printf("file contents %s \n\n\n", (char *)bufferToReadTo);
  myread(fileToTest, bufferToReadTo+100, 100);
  printf("file contents %s \n\n\n", (char *)bufferToReadTo);

  //Reading myio.h
  myread(fileToTest2, bufferToReadTo2, 100);
  printf("file contents %s \n", (char *)bufferToReadTo2);
  myread(fileToTest2, bufferToReadTo2 + 100,100);
  printf("file contents %s \n", (char *)bufferToReadTo2);
  //free both buffers
  //printf("bufferToReadTo is now located at %p \n" , bufferToReadTo);
  free(bufferToReadTo);

  free(bufferToReadTo2);






  //printf("Calling 'myread' on %s, recieved ssize_t %ld.\n",
    //      fileToTest.fileName,
    //      myread(&fileToTest, fileToTest.fileBuffer, BUFFER_SIZE));
  //printf("Has the buffer been written to? %d.\n", fileToTest.bufferWritten);
}
