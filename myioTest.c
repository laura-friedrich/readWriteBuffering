#include "myio.h"

int main(int argc, char *argv[])
{
  FileStruct *fileToTest = myopen("README", O_RDONLY);
  printf("Calling 'myopen' on %s.\n", fileToTest->fileName);

  printf("Calling myread with buf* = 20, count = 100.");
  myread(fileToTest, 20, 100);

  //printf("Calling 'myread' on %s, recieved ssize_t %ld.\n",
    //      fileToTest.fileName,
    //      myread(&fileToTest, fileToTest.fileBuffer, BUFFER_SIZE));
  //printf("Has the buffer been written to? %d.\n", fileToTest.bufferWritten);
}
