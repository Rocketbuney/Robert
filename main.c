#include "sys.h"

size_t strlen(const char *s) {
  const char *p = s;
  while (*p)
    p++;
  return p - s;
}

int robert_main(void) {
  print("hello world\n");

  ASSERT(gpio_ptr, "Failed to map GPIO memory\n");

  INP_GPIO(24);
  OUT_GPIO(24);

  GPIO_SET = 1 << 24;

  return 0;
}
