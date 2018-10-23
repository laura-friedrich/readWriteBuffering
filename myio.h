#ifndef __MYIO_H
#define __MYIO_H

int myopen(const char *pathname, int flags);
int myclose(int fd);
ssize_t myread(int fd, void *buf, size_t count);

#endif
