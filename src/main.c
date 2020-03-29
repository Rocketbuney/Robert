#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "network.h"

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

#ifdef CONTROL
static int text_width(mu_Font font, const char *text, int len) {
  unused(font);
  if (len == -1) { len = strlen(text); }
  return r_get_text_width(text, len);
}

static int text_height(mu_Font font) {
  unused(font);
  return r_get_text_height();
}
#elif ROBOT
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
#endif

int main(int argc, char const *argv[]) {
  unused(argc);
  unused(argv);

  /* init networking thread */
  pthread_t network;
  pthread_create(&network, NULL, networkRunner(), NULL);

#ifdef CONTROL
  /* init SDL and renderer */
  SDL_Init(SDL_INIT_EVERYTHING);
  r_init();

  /* init microui */
  mu_Context *ctx = malloc(sizeof(mu_Context));
  mu_init(ctx);
  ctx->text_width = text_width;
  ctx->text_height = text_height;

  /* main loop */
  for (;;) {
    /* process window input */
    process_input(ctx);
    /* process frame */
    process_frame(ctx);
    /* render */
    render_frame(ctx);
  }
#elif ROBOT
  cursesInit();
  printw("Press escape to exit.\n");

  gpioInit();

  unsigned turnThresh = 15;
  unsigned frontDist;
  unsigned leftDist;
  unsigned rightDist;

  for(;;) {
    motorSpeed = 400;

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
    // mot_updatePins();
    if(getch() == 27)
        break;

    clear();
    delay(250);
  }

  gpioCleanup();
  cursesCleanup();
#endif
  return 0;
}
