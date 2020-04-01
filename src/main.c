#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "dyad.h"

#ifdef CONTROL
#include <SDL2/SDL.h>

#include "microui.h"
#include "Control/renderer.h"
#include "Control/panels.h"
#elif ROBOT
#include <unistd.h>
#include <math.h>
#include <ncurses.h>
#include <wiringPi.h>

#include "Robot/motorController.h"
#include "Robot/distanceSensor.h"
#endif

#define unused(x) ((void) (x))

static dyad_Stream *s;

typedef struct robotObj {
  uint8_t turnThresh, frontDist, leftDist, rightDist, motorSpeed; // 4 bytes
  char motorFlags; // 1 byte
} robotObj;

char *robotObj_serialize(robotObj *obj) {
  char *ser = malloc(sizeof(char) * 5);
  memcpy(&ser[0], &obj->turnThresh, sizeof(uint8_t));
  memcpy(&ser[1], &obj->frontDist, sizeof(uint8_t));
  memcpy(&ser[2], &obj->leftDist, sizeof(uint8_t));
  memcpy(&ser[3], &obj->rightDist, sizeof(uint8_t));
  memcpy(&ser[4], &obj->motorSpeed, sizeof(uint8_t));

  memcpy(&ser[5], &obj->motorFlags, sizeof(char));

  return ser;
}

robotObj *robotObj_deserialize(char *serialized) {
  robotObj *r = malloc(sizeof(robotObj*));
  r->turnThresh = serialized[0];
  r->frontDist = serialized[1];
  r->leftDist = serialized[2];
  r->rightDist = serialized[3];
  r->motorSpeed = serialized[4];
  r->motorFlags = serialized[5];

  return r;
}

#ifdef CONTROL
static mu_Context *ctx;

static int text_width(mu_Font font, const char *text, int len) {
  unused(font);
  if (len == -1) { len = strlen(text); }
  return r_get_text_width(text, len);
}

static int text_height(mu_Font font) {
  unused(font);
  return r_get_text_height();
}

static void onConnect(dyad_Event *e) {
  printf("connected: %s\n", e->msg);
}

static void onData(dyad_Event *e) {
  printf("%s", e->data);
}

static void controlInit() {
  /* init SDL and renderer */
  SDL_Init(SDL_INIT_EVERYTHING);
  r_init();

  /* init microui */
  ctx = malloc(sizeof(mu_Context));
  mu_init(ctx);
  ctx->text_width = text_width;
  ctx->text_height = text_height;

  dyad_addListener(s, DYAD_EVENT_CONNECT, onConnect, NULL);
  dyad_addListener(s, DYAD_EVENT_DATA,    onData,    NULL);
  dyad_connect(s, "10.0.0.147", 80); /* TODO: make IP not hard Coded*/
}

static void controlTick() {
  /* process window input */
  process_input(ctx);
  /* process frame */
  process_frame(ctx);
  /* render */
  render_frame(ctx);
}
#elif ROBOT
static uint8_t turnThresh = 15;
static uint8_t frontDist, leftDist, rightDist;

static void cursesInit() {
  /* Curses Initialisations */
  initscr();
  timeout(0.5);
  refresh();
  raw();
  noecho();
}

static void cursesCleanup() {
  /* ncurses cleanup */
  refresh();
  getch();
  def_prog_mode();
	endwin();
}

static void gpioInit() {
  /* setup gpio pins and motor controller */
  wiringPiSetup();
  mot_initPins();
  dist_initSensor(front);
  dist_initSensor(left);
  dist_initSensor(right);
}

static void gpioCleanup() {
  /* gpio cleanup */
  mot_cleanup();
  dist_cleanup(front);
  dist_cleanup(left);
  dist_cleanup(right);
}

static void onData(dyad_Event *e) {
  dyad_write(e->stream, e->data, e->size);
}

static void onAccept(dyad_Event *e) {
  dyad_addListener(e->remote, DYAD_EVENT_DATA, onData, NULL);
  dyad_writef(e->remote, "Echo server\r\n");
}

static void robotInit() {
  cursesInit();
  printw("Press escape to exit.\n");

  gpioInit();

  dyad_addListener(s, DYAD_EVENT_ACCEPT, onAccept, NULL);
  dyad_listen(s, 80);
}

static void robotTick() {
  motorSpeed = 300;

  frontDist = roundf(dist_getDistance(front) + dist_getDistance(front) + dist_getDistance(front)) / 3;
  leftDist = roundf(dist_getDistance(left) + dist_getDistance(left) + dist_getDistance(left)) / 3;
  rightDist = roundf(dist_getDistance(right) + dist_getDistance(right) + dist_getDistance(right)) / 3;

  printw("Front: %icm, Left: %icm, Right: %icm\n", frontDist, leftDist, rightDist);

  if(frontDist >= turnThresh) {
    motorFlags = RIGHT_FORWARD | LEFT_FORWARD | STBY_HIGH;
    printw("straight\n");
  } else if(leftDist <= turnThresh && rightDist > turnThresh) {
    printw("turning left\n");
    motorFlags = RIGHT_BACKWARD | LEFT_FORWARD | STBY_HIGH;
  } else if(rightDist <= turnThresh && leftDist > turnThresh) {
    printw("turning right\n");
    motorFlags = RIGHT_FORWARD | LEFT_BACKWARD | STBY_HIGH;
  } else {
    printw("backing up");
    motorFlags = RIGHT_BACKWARD | LEFT_BACKWARD | STBY_HIGH;
  }

  /* apply the motor mask to the board */
  mot_updatePins();
  if(getch() == 27) {
    gpioCleanup();
    cursesCleanup();
    exit(0);
  }

  clear();
  delay(250);
}
#endif

int main(int argc, char const *argv[]) {
  unused(argc);
  unused(argv);

  dyad_init();
  dyad_setUpdateTimeout(0);
  s = dyad_newStream();

  robotObj obj = (robotObj){ 16, 32, 64, 128, 128, 0 };
  char *serObj = robotObj_serialize(&obj);

  robotObj *newObj = robotObj_deserialize(serObj);
  printf("%u %u %u %u %u\n", newObj->turnThresh, newObj->frontDist, newObj->leftDist, newObj->rightDist, newObj->motorSpeed );

#ifdef CONTROL
  controlInit();
#elif ROBOT
  robotInit();
#endif

  for (;;) {
    dyad_update();

    #ifdef CONTROL
    controlTick();
    #elif ROBOT
    robotTick();
    #endif
  }

  dyad_shutdown();

  #ifdef ROBOT
exit:
  gpioCleanup();
  cursesCleanup();
  #endif
  return 0;
}
