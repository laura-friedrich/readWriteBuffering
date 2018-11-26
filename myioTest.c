#include "myio.h"

int main(int argc, char *argv[])

{
  //fflush(stdout);
  //void *bufferToReadTo = malloc(10000);
  void *bufferToReadTo2 = malloc(10000);

  //Have two open files at once

  //opening README
  //FileStruct *fileToTest = myopen("sourceTest", O_RDONLY);

  //Opening myio.h
  FileStruct *fileToTest2 = myopen("sourceTest", O_RDONLY);
  //open("sourceTest", O_RDONLY);

  //Opening Test file to write to
  FileStruct *fileToTest3 = myopen("test", O_RDWR);
  int writeFile = open("test2", O_RDWR);
  //FileStruct *fileToTest4 = myopen("test2", O_RDWR);

  //Reading README
//  myread(fileToTest, bufferToReadTo, 100);
  //printf("file contents %s \n\n\n", (char *)bufferToReadTo);

  //continue reading from the file but without calling another system call
  //myread(fileToTest, (char *)bufferToReadTo+100, 100);


  //Reading myio.h
  myread(fileToTest2, bufferToReadTo2, 8000);
  //read(open("sourceTest", O_RDONLY), bufferToReadTo,8000);


  //int compValue = memcmp(bufferToReadTo, bufferToReadTo2, 8000);
  //printf("%d", compValue);

  //printf("file contents of myRead\n %s \n\n\n", (char *)bufferToReadTo2);

  //printf("file contents of read is \n %s \n\n\n", (char *)bufferToReadTo);
  //continue reading from the file but without calling another system call
  //myread(fileToTest2, (char *)bufferToReadTo2 + 100,100);
  //printf("file contents %s \n", (char *)bufferToReadTo2);

  mywrite(fileToTest3, bufferToReadTo2,60);
  mywrite(fileToTest3, bufferToReadTo2,80);
  write(writeFile, bufferToReadTo2, 60);
  write(writeFile, bufferToReadTo2, 80);

  //myclose(fileToTest);
  myclose(fileToTest2);
  myclose(fileToTest3);


  //free both buffers
  //free(bufferToReadTo);
  free(bufferToReadTo2);

}
