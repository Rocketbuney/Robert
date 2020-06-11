#include "sys.h"

extern void *gpio_ptr;

size_t strlen(const char *s) {
  const char *p = s;
  while (*p)
    p++;
  return p - s;
}

int robert_main(void) {
  print("hello world\n");
  ASSERT(gpio_ptr, "Failed to map GPIO memory\n");

  return 0;
}
