#include "distanceSensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

void dist_initSensor(dist_sensorID sensor) {
    int trig;
    int echo;

    switch(sensor) {
        case front:
            trig = 0;
            echo = 4;
            break;
        case left:
            trig = 3;
            echo = 6;
            break;
        case right:
            trig = 2;
            echo = 5;
            break;
    }

    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);

    digitalWrite(echo, 0);
    delay(500);
}

float dist_getDistance(dist_sensorID sensor) {
    int trig;
    int echo;

    float startTime;
    float endTime;

    switch(sensor) {
        case front:
            trig = 0;
            echo = 4;
            break;
        case left:
            trig = 3;
            echo = 6;
            break;
        case right:
            trig = 2;
            echo = 5;
            break;
    }

    digitalWrite(trig, 1);
    delay(0.0001);
    digitalWrite(trig, 0);
    float startOfMeasurement = micros();

    while(digitalRead(echo) == 0  && micros() - startOfMeasurement < 30000)
        startTime = micros();

    while(digitalRead(echo) == 1)
        endTime = micros();

    return  (((endTime - startTime) / 2) / 29) < 0 ? 0 : ((endTime - startTime) / 2) / 29;
}

void dist_cleanup(dist_sensorID sensor) {
    int trig;
    int echo;

    switch(sensor) {
        case front:
            trig = 0;
            echo = 4;
            break;
        case left:
            trig = 3;
            echo = 6;
            break;
        case right:
            trig = 2;
            echo = 5;
            break;
    }

    pinMode(trig, OUTPUT);
    pinMode(echo, OUTPUT);

    digitalWrite(trig, 0);
    digitalWrite(echo, 0);
}
