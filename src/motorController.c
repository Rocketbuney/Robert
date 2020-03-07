#include "motorController.h"

#include <wiringPi.h>

char motorFlags = 0x00;
unsigned motorSpeed = 600;

void mot_initPins() {
    pinMode(PWMA, PWM_OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(PWMB, PWM_OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    pinMode(STBY, OUTPUT);
}

void mot_updatePins() {
    if ((motorFlags & RIGHT_FORWARD) > 0) {
        digitalWrite(BIN1, 0);
        digitalWrite(BIN2, 1);
    } else if((motorFlags & RIGHT_BACKWARD) > 0) {
        digitalWrite(BIN1, 1);
        digitalWrite(BIN2, 0);
    }

    if ((motorFlags & LEFT_FORWARD) > 0) {
        digitalWrite(AIN1, 1);
        digitalWrite(AIN2, 0);
    } else if((motorFlags & LEFT_BACKWARD) > 0) {
        digitalWrite(AIN1, 0);
        digitalWrite(AIN2, 1);
    }

    if ((motorFlags & STBY_HIGH) > 0) {
        digitalWrite(STBY, 1);
    } else if ((motorFlags & STBY_LOW) > 0) {
        digitalWrite(STBY, 0);
    }

    pwmWrite(PWMB, motorSpeed);
    pwmWrite(PWMA, motorSpeed);
}

void mot_cleanup() {
    pinMode(PWMB, INPUT);
    pinMode(BIN1, INPUT);
    pinMode(BIN2, INPUT);
    pinMode(AIN1, INPUT);
    pinMode(AIN2, INPUT);
    pinMode(PWMA, INPUT);
    pinMode(STBY, INPUT);

    digitalWrite(PWMB, LOW);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    digitalWrite(PWMA, LOW);
    digitalWrite(STBY, LOW);
}