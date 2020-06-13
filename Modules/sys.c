#include "sys.h"

static struct block_t {
  size_t size;          /* size of block */
  uint8_t free;             /* weather the block is free or not */
  struct block_t *next; /* pointer to the next block */
};

#define kMemSize 5000
static char memory[kMemSize]; /* 5k */
static struct block_t *freeList = (void*)memory; /* initialise the pointer to point to starting address of the memory */

/* init memory */
static void memInit() {
  /* Init first metadata block */
  freeList->size = kMemSize - sizeof(struct block_t);
  freeList->free = 1;
  freeList->next = NULL;
}

/* split free block */
static void split(struct block_t *fittingSlott, size_t s) {
  /* new block allocation by slpitting free block, use first fit */
  struct block_t *new = (void*)((void*)fittingSlott + s + sizeof(struct block_t));
  new->size = (fittingSlott->size) - s - sizeof(struct block_t);
  new->free = 1;
  new->next = fittingSlott->next;

  fittingSlott->size = s;
  fittingSlott->free = 0;
  fittingSlott->next = new;
}

/* merge consecutive free memory blocks */
static void merge() {
  struct block_t *cur, *prev;
  cur = freeList;

  while(cur->next != NULL) {
    if(cur->free && cur->next->free) {
      cur->size += cur->next->size + sizeof(struct block_t);
      cur->next = cur->next->next;
    }

    prev = cur;
    cur = cur->next;
  }
}

void *malloc(size_t size) {
  struct block_t *cur, *prev;

  if(!freeList->size) {
    memInit();
    print("Initialised memory\n");
  }

  cur = freeList;
  while((cur->size < size || cur->free == 0) && cur->next != NULL) {
    prev = cur;
    cur = cur->next;
    print("Block checked.\n");
  }

  if(cur->size == size) {
    cur->free = 0;
    print("Block allocated.\n");
    return (void*)cur++;
  } else if(cur->size > size + sizeof(struct block_t)){
    split(cur, size);
    print("Block allocated with slpit.\n");
    return (void*)cur++;
  } else {
    print("Insufficient memory.\n");
    return NULL;
  }
}

void free(void *p) {
  if((void*)memory <= p && p <= (void*)(memory+kMemSize)) {
    struct block_t *cur = p;
    cur--;
    cur->free = 1;
    merge();

    print("Block Freed\n");
    return;
  }

  print("Invalid pointer passed\n");
}

void *realloc(void *ptr, size_t size_new, size_t size_old) {
  if(size_new == 0) { /* no length supplied */
    return NULL;
  } else if(!ptr) { /* if pointer doesnt exist, allocate */
    return malloc(size_new);
  } else if(size_new <= size_old) { /* only allocate more of necessary */
    return ptr;
  } else {
    ASSERT(ptr, "Invalid pounter passed to realloc. Quitting.\n");
    void *_ptr = malloc(size_new);

    if(_ptr) {
      memcpy(_ptr, ptr, size_old);
      free(ptr);
    }

    return _ptr;
  }
}

size_t strlen(const char *s) {
  const char *p = s;
  while (*p)
    p++;
  return p - s;
}

char *strcpy(char *dest, const char *src) {
  unsigned n = 0;
  while(*src) {
    dest[n] = *src;
    n++;
    src++;
  }

  return dest;
}

char* strcat(char* destination, const char* source) {
  char* ptr = destination + strlen(destination);

  while (*source != '\0')
    *ptr++ = *source++;

  *ptr = '\0';
  return destination;
}

void *memset(void *s, int c, size_t n) {
  unsigned char *ptr = s;
  while(n-- > 0) {
    *ptr++ = c;
  }

  return s;
}

void *memcpy(void *dest, const void *src, size_t n) {
  size_t i = 0;
  char *_src = (char*)src, *_dest = (char*)dest;

  while(i < n) {
    _dest[i] = _src[i];
    i++;
  }

  return _dest;
}
