#ifndef ROBOTOPT_H
#define ROBOTOPT_H

#include <stdint.h>

typedef struct robotOpt_t {
  uint8_t turnThresh, frontDist, leftDist, rightDist, motorSpeed;
  char motorFlags;
} robotOpt_t;

robotOpt_t *robotOpt_create();

char *robotOpt_serialize(robotOpt_t *);
robotOpt_t *robotOpt_deserialize(char *);

#endif
