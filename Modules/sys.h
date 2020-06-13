#ifndef SYS_H
#define SYS_H

/* Data type definitions */
#define NULL 0L

typedef unsigned short uint16_t;
typedef unsigned long long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef unsigned int size_t;
typedef unsigned int time_t;
typedef int ssize_t;

struct timespec {
  time_t tv_sec;        /* seconds */
  long   tv_nsec;       /* nanoseconds */
};

/***** system calls ******/
#define CLOCK_MONOTONIC_RAW 4
int nanosleep(const struct timespec *req, struct timespec *rem);
int clock_gettime(uint32_t clk_id, struct timespec *tp);

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

struct block_t;
void *mmap(void *addr, size_t length, int prot, int flags, int fd, int  offset);
int munmap(void *addr, size_t length);

void *malloc(size_t size);
void free(void *p);

/* string related */
size_t strlen(const char *);
void *memset(void *s, int c, size_t n);

/* print */
#define print(s) write(1, s, strlen(s))
#define printn(s, n) write(1, s, n)

/* assert */
#define ASSERT(expr, msg) \
    if (!(expr)) { \
        print(msg); \
        exit(-1); }

#define UNUSED(x) (void)(x)

/* GPIO macros */
extern uint32_t *gpio_ptr; /* defined by assembly */

#define INP_GPIO(g) *(gpio_ptr+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio_ptr+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio_ptr+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio_ptr+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio_ptr+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio_ptr+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio_ptr+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio_ptr+38) // Pull up/pull down clock

#endif
