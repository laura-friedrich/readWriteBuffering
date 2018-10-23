#include "myio.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
  //printf("%d", myopen("path", O_WRONLY));
}

/* Accepts Flags:
  - O_CREAT
  - O_WRONLY
  - O_RDONLY
  - O_RDWR
  - O_TRUNC
*/

// Calls system call open
int myopen(const char *pathname, int flags){
  return open(pathname, flags);
}
