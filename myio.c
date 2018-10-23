#include <sys/types.h>
#include <stdio.h>
// Needed for open
#include <fcntl.h>
#include <sys/stat.h>
// Needed for close, read
#include <unistd.h>
#include <stdlib.h>
// Include our header file
#include "myio.h"

#define BUFFER_SIZE 256


int main(int argc, char *argv[])
{
  //char *read_buffer = (char*) malloc (BUFFER_SIZE);
  printf("%d\n", myclose(myopen("path", O_CREAT)));
  char* contents[BUFFER_SIZE];
  printf("%ld\n", myread(myopen("myio.c", O_RDONLY), contents, BUFFER_SIZE));
}

/* Accepts Flags:
  - O_CREAT
  - O_WRONLY
  - O_RDONLY
  - O_RDWR
  - O_TRUNC
*/

ssize_t myread(int fd, void *buf, size_t count){
  read(fd, buf, BUFFER_SIZE);
  return 0;
}

// Calls system call open
int myopen(const char *pathname, int flags){
  return open(pathname, flags);
}

// Calls system call close
int myclose(int fd){
  return close(fd);
}
