#ifndef __MYIO_H
#define __MYIO_H

typedef struct FileStruct;
int myopen(const char *pathname, int flags);
//int myopen(struct File fd, int flags);

int myclose(int fd);
//ssize_t myread(int fd, void *buf, size_t count);
ssize_t myread(struct FileStruct fd, void *buf, size_t count);

#endif
