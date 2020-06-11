#ifndef SYS_H
#define SYS_H

/* Data type definitions */
#define NULL 0L

typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef unsigned int size_t;
typedef int ssize_t;

/***** system calls ******/
/* File I/O */
#define	O_RDWR 2
#define	O_SYNC 0x0080

ssize_t write(int fd, const void *buf, size_t count);
int open(const char *pathname, int flags);
int close(int fd);
void exit(int ret);

/* Memory Management */
#define PROT_READ   0x1
#define PROT_WRITE  0x2
#define MAP_SHARED  0x01
#define MAP_FAILED	((void *)-1)
#define MAP_ANON    0x20

void *mmap(void *addr, size_t length, int prot, int flags, int fd, int  offset);
int munmap(void *addr, size_t length);

/* string related */
size_t strlen(const char *);

/* print */
#define print(s) write(1, s, strlen(s))
#define printn(s, n) write(1, s, n)

/* assert */
#define ASSERT(expr, msg) \
    if (!(expr)) { \
        print(msg); \
        exit(-1); }

#endif
