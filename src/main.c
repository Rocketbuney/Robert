#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "dyad.h"

#include "robotOpt.h"

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
  robotOpt_t *opt = robotOpt_deserialize(e->data);

  printf("%u %u %u\n", opt->frontDist, opt->leftDist, opt->rightDist);
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

  robotOpt_t *opt = robotOpt_create();
  opt->frontDist = frontDist;
  opt->leftDist = leftDist;
  opt->rightDist = rightDist;

  char *s = robotOpt_serialize(opt);
  dyad_writef(e->remote, "%s\r\n", s);
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
