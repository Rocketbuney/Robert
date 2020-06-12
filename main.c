#include "sys.h"

size_t strlen(const char *s) {
  const char *p = s;
  while (*p)
    p++;
  return p - s;
}

int robert_main(void) {
  ASSERT(gpio_ptr, "Failed to map GPIO memory\n");

  struct timespec sleepDuration = { 2, 0.0 };

  print("hello world\n");
  nanosleep(&sleepDuration, NULL);
  print("hello world\n");

  return 0;
}
