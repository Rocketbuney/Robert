#ifndef ROBOTOPT_H
#define ROBOTOPT_H

typedef struct {
  uint8_t turnThresh, frontDist, leftDist, rightDist, motorSpeed;
  char motorFlags;
} robotOpt_t

robotOpt_t *robotOpt_create();

char *robotOpt_serialize(robotOpt_t *);
robotOpt_t *robotOpt_deserialize(char *);

#endif
