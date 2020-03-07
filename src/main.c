#include <ncurses.h>
#include <wiringPi.h>
#include "motorController.h"

int main(void) {
    /* Curses Initialisations */
    initscr();
	refresh();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    wiringPiSetup();
    mot_initPins();
    
    printw("Press escape to exit.");

    int ch;
    while((ch = getch()) != 27) {
        motorSpeed = 300;

        switch (ch) {
        case KEY_UP:
            motorFlags = RIGHT_FORWARD | LEFT_FORWARD | STBY_HIGH;
            break;
        case KEY_DOWN:
            motorFlags = RIGHT_BACKWARD | LEFT_BACKWARD | STBY_HIGH;
            break;
        case KEY_LEFT:
            motorSpeed /= 1.75;
            motorFlags = LEFT_BACKWARD | RIGHT_FORWARD | STBY_HIGH;
            break;
        case KEY_RIGHT:
            motorSpeed /= 1.75;
            motorFlags = RIGHT_BACKWARD | LEFT_FORWARD | STBY_HIGH;
            break;
        default:
            motorFlags = 0x00 | STBY_LOW;
            break;
        }

        mot_updatePins();
    }

    mot_cleanup();
    
    /* ncurses cleanup */
    refresh();
    getch();
    def_prog_mode();
	endwin();
    return 0;
}
