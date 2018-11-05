#include "myio.h"

int main(int argc, char *argv[])
{
  struct FileStruct fileToTest;
  fileToTest.fileName = argv[1];
  fileToTest.position = 0;
  fileToTest.bufferWritten = 0;
  fileToTest.fileDescriptor = myopen(fileToTest.fileName, O_RDONLY);

  printf("Calling 'myopen' on %s, recieved int %d.\n",
          fileToTest.fileName,
          myopen(fileToTest.fileName, O_CREAT));
  printf("Calling 'myread' on %s, recieved ssize_t %ld.\n",
          fileToTest.fileName,
          myread(&fileToTest, fileToTest.fileBuffer, BUFFER_SIZE));
  printf("Has the buffer been written to? %d.\n", fileToTest.bufferWritten);
}
