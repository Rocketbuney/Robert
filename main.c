#include "Modules/sys.h"

int robert_main(void) {
  ASSERT(gpio_ptr, "Failed to map GPIO memory\n");

  struct timespec *sleepDuration = (struct timespec*)malloc(sizeof(struct timespec));
  memset(sleepDuration, 0, sizeof(*sleepDuration));
  sleepDuration->tv_sec = 2.0;
  sleepDuration->tv_nsec = 0.0;

  print("Print 1\n");
  nanosleep(sleepDuration, NULL);
  print("Print 2\n");

  free(sleepDuration);
  return 0;
}
