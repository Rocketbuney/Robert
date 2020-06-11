#include "sys.h"

extern void *gpio_ptr;

int robert_main(void) {
  write(1, "hello world\n", 12);

  if(!gpio_ptr) {
    write(1, "failed to load memory region\n", 29);
    return -1;
  }

  return 0;
}
