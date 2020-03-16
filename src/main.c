#include <ncurses.h>
#include <wiringPi.h>
#include <unistd.h>
#include "motorController.h"
#include "distanceSensor.h"

WINDOW *inputWin;
int xMax, yMax;

static void cursesInit() {
    /* Curses Initialisations */
    initscr();
    timeout(0.5);
	refresh();
    raw();
    noecho();

    inputWin = newwin(6, xMax-12, yMax - 8, 5);
    keypad(inputWin, TRUE);
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
    dist_initSensor();
}

static void gpioCleanup() {
    /* gpio cleanup */
    mot_cleanup();
    dist_cleanup();
}

int main(void) {
    cursesInit();
    printw("Press escape to exit.\n");
    
    gpioInit();

    float dist;
    int isTurning;
    int turnThresh = 3;

    for(;;) {
        motorSpeed = 400;
        dist = dist_getDistance();

        if(isTurning)
            isTurning = 0;

        if(dist > turnThresh) 
            motorFlags = RIGHT_FORWARD | LEFT_FORWARD | STBY_HIGH;
        else if(dist < turnThresh) {
            isTurning = 1;
            motorFlags = RIGHT_FORWARD | LEFT_BACKWARD | STBY_HIGH;
        }

        /* apply the motor mask to the board */
        mot_updatePins();

        if(isTurning)
            delay(250);
        if(getch() == 27) 
            break;
    }

    gpioCleanup();
    cursesCleanup();
    return 0;
}