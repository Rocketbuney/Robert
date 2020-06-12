#include "sys.h"

#define ECHO 16
#define TRIG 20

size_t strlen(const char *s) {
  const char *p = s;
  while (*p)
    p++;
  return p - s;
}

int robert_main(void) {
  ASSERT(gpio_ptr, "Failed to map GPIO memory\n");
  struct timespec sleepDuration = { 2, 0.0 };
  struct timespec sensorDuration = { 0, 10000.0 };
  struct timespec pulseStart, pulseEnd;

  long dist;

  INP_GPIO(ECHO);
  INP_GPIO(TRIG);
  OUT_GPIO(TRIG);
  nanosleep(&sleepDuration, NULL);

  /* Pulse trigger pin */
  GPIO_SET = 1 << TRIG;
  nanosleep(&sensorDuration, NULL);
  GPIO_CLR = 1 << TRIG;

  /* Accurately get pulse start and end */
  while(GET_GPIO(ECHO) == 0) {
    clock_gettime(CLOCK_MONOTONIC_RAW, &pulseStart);  /* get time */
  }

  while(GET_GPIO(ECHO) == (1 << ECHO)) {
    clock_gettime(CLOCK_MONOTONIC_RAW, &pulseEnd);  /* get time */
  }

  /* calculate distance */
  long pulseEnd_s = (pulseEnd.tv_sec * 1000000) + (pulseEnd.tv_nsec / 1000L);
  long pulseStart_s = (pulseStart.tv_sec * 1000000) + (pulseStart.tv_nsec / 1000L);
  long long dur = (pulseEnd_s - pulseStart_s); /* in seconds */
  dist = (dur / 2) / 29;

  /* TODO: print dist */

  return 0;
}
