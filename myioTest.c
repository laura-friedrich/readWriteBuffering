#include "myio.h"

int main(int argc, char *argv[])

{
  //fflush(stdout);
  void *bufferToReadTo = malloc(10000);
  void *bufferToReadTo2 = malloc(10000);

  //Have two open files at once

  //opening README
  //FileStruct *fileToTest2 = myopen("sourceTest", O_CREAT | O_RDONLY);
  //open("sourceTest", O_RDONLY);

  //Opening Test file to write to
  FileStruct *fileToTest3 = myopen("test", O_CREAT | O_RDWR);
  FileStruct *fileToTest2 = myopen("sourceTest", O_CREAT | O_RDWR);
  int writeFile = open("test2", O_CREAT | O_RDWR, 0666);
  //FileStruct *fileToTest4 = myopen("test2", O_RDWR);

  int fileToRead = open("sourceTest", O_RDONLY);

  //read(fileToRead,bufferToReadTo2,10);


    /*
  mywrite(fileToTest3, bufferToReadTo2,2000);
  write(writeFile, bufferToReadTo2, 2000);
  myread(fileToTest3, bufferToReadTo2,2000);
  read(writeFile, bufferToReadTo2, 2000);
  mywrite(fileToTest3, bufferToReadTo2,2000);
  write(writeFile, bufferToReadTo2, 2000);
  */

  myread(fileToTest2,bufferToReadTo2,3);
  read(fileToRead, bufferToReadTo, 3);
  myread(fileToTest2,(char *)bufferToReadTo2+3,5);
  read(fileToRead,(char *) bufferToReadTo+3, 5);
  myread(fileToTest2,(char *)bufferToReadTo2 +8,13);
  read(fileToRead, (char *)bufferToReadTo +8, 13);

  printf("my string is %s \n",(char *) bufferToReadTo2);
  printf("The Correctstring is %s \n", (char *)bufferToReadTo);
  int compValue = memcmp(bufferToReadTo, bufferToReadTo2, 9000);
  printf("%d", compValue);

  /*

  mywrite(fileToTest3,(char *) bufferToReadTo2+2,4);
  mywrite(fileToTest3, bufferToReadTo2,9);
  //mywrite(fileToTest3, bufferToReadTo2,9);
  write(writeFile, (char*) bufferToReadTo2+2, 4);


  write(writeFile, bufferToReadTo2, 9);
  //write(writeFile, bufferToReadTo2, 9);
  myread(fileToTest3,bufferToReadTo2,4);
  read(writeFile, bufferToReadTo2, 4);
  //myread(fileToTest3, bufferToReadTo2, 2);
  //read(writeFile, bufferToReadTo2,2);
  mywrite(fileToTest3, bufferToReadTo2, 2);
  write(writeFile, bufferToReadTo2, 2);

  myread(fileToTest3, bufferToReadTo2, 2);
  read(writeFile, bufferToReadTo2,2);

  */


  //Reading README


  /*read(fileToRead,bufferToReadTo2,200);
  myread(fileToTest, bufferToReadTo, 200);
  myread(fileToTest, (char *) bufferToReadTo+200, 2000);
  read(fileToRead, (char *) bufferToReadTo2 +200,2000);
  read(fileToRead, (char *)bufferToReadTo2 +2200,6000);
  myread(fileToTest, (char *) bufferToReadTo +2200, 6000);



  mywrite(fileToTest3, bufferToReadTo2,2000);
  write(writeFile, bufferToReadTo2, 2000);
  myread(fileToTest3, bufferToReadTo2,2000);
  read(writeFile, bufferToReadTo2, 2000);
  mywrite(fileToTest3, bufferToReadTo2,2000);
  write(writeFile, bufferToReadTo2, 2000);

  //myread(fileToTest3,bufferToReadTo2,200);
  //read(writeFile, bufferToReadTo, 200);

  mywrite(fileToTest3, bufferToReadTo2,2050);
  mywrite(fileToTest3, bufferToReadTo2,3060);
  mywrite(fileToTest3, bufferToReadTo2,2000);
  write(writeFile, bufferToReadTo2, 2050);
  write(writeFile, bufferToReadTo2, 3060);
  write(writeFile, bufferToReadTo2, 2000);
  myread(fileToTest3, bufferToReadTo2, 200);
  read(writeFile, bufferToReadTo,200);
  mywrite(fileToTest3, bufferToReadTo2, 200);
  write(writeFile, bufferToReadTo2, 200);


  int compValue = memcmp(bufferToReadTo, bufferToReadTo2, 9000);
  printf("%d", compValue);

  */

  //read(fileToTest2, bufferToReadTo2,100);
  //printf("file contents %s \n\n\n", (char *)bufferToReadTo);
  //printf("file contents %s \n\n\n", (char *)bufferToReadTo2);

  //continue reading from the file but without calling another system call
  //myread(fileToTest, (char *)bufferToReadTo+100, 100);

/*
  //Reading myio.h
  myread(fileToTest2, bufferToReadTo2, 8000);
  read(open("sourceTest", O_RDONLY), bufferToReadTo,8000);


  //int compValue = memcmp(bufferToReadTo, bufferToReadTo2, 8000);
  //printf("%d", compValue);

  //printf("file contents of myRead\n %s \n\n\n", (char *)bufferToReadTo2);

  //printf("file contents of read is \n %s \n\n\n", (char *)bufferToReadTo);
  //continue reading from the file but without calling another system call
  //myread(fileToTest2, (char *)bufferToReadTo2 + 100,100);
  //printf("file contents %s \n", (char *)bufferToReadTo2);

  mywrite(fileToTest3, bufferToReadTo2,2050);
  mywrite(fileToTest3, bufferToReadTo2,3060);
  mywrite(fileToTest3, bufferToReadTo2,2000);
  write(writeFile, bufferToReadTo2, 2050);
  write(writeFile, bufferToReadTo2, 3060);
  write(writeFile, bufferToReadTo2, 2000);
  //myread(fileToTest3, bufferToReadTo2, 80);
  //read(writeFile, bufferToReadTo2,80);
  mywrite(fileToTest3, bufferToReadTo2, 200);
  write(writeFile, bufferToReadTo2, 200);

  //myclose(fileToTest);
  myclose(fileToTest2);
  myclose(fileToTest3);
  close(writeFile);


  fileToTest2 = myopen("sourceTest", O_RDONLY);
  open("sourceTest", O_RDONLY);

  //Opening Test file to write to


  fileToTest3 = myopen("test", O_RDWR);
  writeFile = open("test2", O_RDWR);
  mywrite(fileToTest3, bufferToReadTo2,400);
  mywrite(fileToTest3, bufferToReadTo2,300);
  mywrite(fileToTest3, bufferToReadTo2,250);
  write(writeFile, bufferToReadTo2, 400);
  write(writeFile, bufferToReadTo2, 300);
  write(writeFile, bufferToReadTo2, 250);

*/
  //myclose(fileToTest2);
  myclose(fileToTest3);
  myclose(fileToTest2);
  close(writeFile);

  //free both buffers
  free(bufferToReadTo);
  free(bufferToReadTo2);

}
