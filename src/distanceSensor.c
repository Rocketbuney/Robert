#include "distanceSensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

enum dist_pinAssignments {
    TRIG = 8,
    ECHO = 9,
};

static volatile long startTimeUsec;
static volatile long endTimeUsec;

void dist_initSensor() {
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    digitalWrite(TRIG, 0);
    digitalWrite(ECHO, 0);

    printw("Sensor Balancing\n");
    sleep(1);
    printw("Sensor Balanced.\n");
}

static void recordPulseLength() {
    startTimeUsec = micros();
    while (digitalRead(ECHO) == HIGH);
    endTimeUsec = micros();
}

float dist_getDistance() {
    // Send trig pulse
    // Triggering the sensor for 10 microseconds will cause it to send out
    // 8 ultrasonic (40Khz) bursts and listen for the echos.
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    int now = micros();
    // Wait for echo start
    // The sensor will raise the echo pin high for the length of time that it took
    // the ultrasonic bursts to travel round trip.
    while (digitalRead(ECHO) == LOW && micros() - now < 30000);
    recordPulseLength();

    long travelTimeUsec = endTimeUsec - startTimeUsec;
    double distanceMeters = 100 * ((travelTimeUsec / 1000000.0) * 340.29) / 2;

    //Wait for echo end
    long startTime = micros();
    while (digitalRead(ECHO) == HIGH);
    long travelTime = micros() - startTime;

    return distanceMeters;
}

void dist_cleanup() {
    digitalWrite(TRIG, 0);

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, OUTPUT);
}
