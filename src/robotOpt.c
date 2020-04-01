#include "robotOpt.h"

#include <string.h>
#include <stdlib.h>

robotOpt_t *robotOpt_create() {
  robotOpt_t *r = malloc(sizeof(robotOpt_t*));
  memset(r, 0, sizeof(robotOpt_t*));

  return r;
}

char *robotOpt_serialize(robotOpt_t *r) {
  char *s = malloc(sizeof(char) * 5);
  memcpy(&s[0], &r->turnThresh, sizeof(uint8_t));
  memcpy(&s[1], &r->frontDist, sizeof(uint8_t));
  memcpy(&s[2], &r->leftDist, sizeof(uint8_t));
  memcpy(&s[3], &r->rightDist, sizeof(uint8_t));
  memcpy(&s[4], &r->motorSpeed, sizeof(uint8_t));

  memcpy(&s[5], &r->motorFlags, sizeof(char));

  return s;
}

robotOpt_t *robotOpt_deserialize(char *s) {
  robotOpt_t *r = robotOpt_create();
  r->turnThresh = s[0];
  r->frontDist = s[1];
  r->leftDist = s[2];
  r->rightDist = s[3];
  r->motorSpeed = s[4];
  r->motorFlags = s[5];

  return r;
}
