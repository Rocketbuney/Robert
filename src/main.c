#include <ncurses.h>
#include <wiringPi.h>
#include <unistd.h>
#include <math.h>
#include "motorController.h"
#include "distanceSensor.h"

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

int main(void) {
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
        mot_updatePins();
        if(getch() == 27) 
            break;

        clear();
        delay(250);
    }

    gpioCleanup();
    cursesCleanup();
    return 0;
}